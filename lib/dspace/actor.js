


class MaterializationContext {
    constructor () {
      this.diffs = {}
      this.children = {}
    }
  
    /**
     * Updates `diff` with the properties from `data`. The object `data` should be
     * of the form `{value: objectId, link: true}` if the value is a reference to
     * another object, of the form `{value: timestamp, datatype: 'timestamp'}` if
     * the value is a Date object, and of the form `{value: primitiveValue}`
     * if the value is a primitive value.
     */
    unpackValue(parentId, diff, data) {
      Object.assign(diff, data)
      if (data.link) {
        this.children[parentId].push(data.value)
      }
    }
  
    /**
     * Unpacks `conflicts`: if it's an Immutable.js Map object (where keys are
     * actor IDs and values are primitive or object values), updates `diff` to
     * describe the conflicts.
     */
    unpackConflicts(parentId, diff, conflicts) {
      if (conflicts) {
        diff.conflicts = []
  
        for (let [actor, value] of conflicts) {
          let conflict = {actor}
          this.unpackValue(parentId, conflict, value)
          diff.conflicts.push(conflict)
        }
      }
    }
  
    /**
     * Updates `this.diffs[objectId]` to contain the patch necessary to
     * instantiate the map object with ID `objectId`.
     */
    instantiateMap(opSet, objectId, type) {
      let diffs = this.diffs[objectId]
      if (objectId !== OpSet.ROOT_ID) {
        diffs.push({obj: objectId, type, action: 'create'})
      }
  
      const conflicts = OpSet.getObjectConflicts(opSet, objectId, this)
  
      for (let key of OpSet.getObjectFields(opSet, objectId)) {
        let diff = {obj: objectId, type, action: 'set', key}
        this.unpackValue(objectId, diff, OpSet.getObjectField(opSet, objectId, key, this))
        this.unpackConflicts(objectId, diff, conflicts.get(key))
        diffs.push(diff)
      }
    }
  
    /**
     * Updates `this.diffs[objectId]` to contain the patch necessary to
     * instantiate the list or text object with ID `objectId`.
     */
    instantiateList(opSet, objectId, type) {
      let diffs = this.diffs[objectId], maxCounter = 0
      diffs.push({obj: objectId, type, action: 'create'})
  
      for (let item of OpSet.listIterator(opSet, objectId, this)) {
        const {index, elemId, value, conflicts} = item
        maxCounter = Math.max(maxCounter, parseElemId(elemId).counter)
  
        if (index !== undefined) {
          let diff = {obj: objectId, type, action: 'insert', index, elemId}
          this.unpackValue(objectId, diff, value)
          this.unpackConflicts(objectId, diff, conflicts)
          diffs.push(diff)
        }
      }
      diffs.push({obj: objectId, type, action: 'maxElem', value: maxCounter})
    }
  
    /**
     * Called by OpSet.getOpValue() when recursively instantiating an object in
     * the document tree. Updates `this.diffs[objectId]` to contain the patch
     * necessary to instantiate the object, and returns
     * `{value: objectId, link: true}` (which is used to update the parent object).
     */
    instantiateObject(opSet, objectId) {
      if (this.diffs[objectId]) return {value: objectId, link: true}
  
      const isRoot = (objectId === OpSet.ROOT_ID)
      const objType = opSet.getIn(['byObject', objectId, '_init', 'action'])
      this.diffs[objectId] = []
      this.children[objectId] = []
  
      if (isRoot || objType === 'makeMap') {
        this.instantiateMap(opSet, objectId, 'map')
      } else if (objType === 'makeTable') {
        this.instantiateMap(opSet, objectId, 'table')
      } else if (objType === 'makeList') {
        this.instantiateList(opSet, objectId, 'list')
      } else if (objType === 'makeText') {
        this.instantiateList(opSet, objectId, 'text')
      } else {
        throw new RangeError(`Unknown object type: ${objType}`)
      }
      return {value: objectId, link: true}
    }
  
    /**
     * Constructs the list of all `diffs` necessary to instantiate the object tree
     * whose root is the object with ID `objectId`.
     */
    makePatch(objectId, diffs) {
      for (let childId of this.children[objectId]) {
        this.makePatch(childId, diffs)
      }
      for (let d of this.diffs[objectId]) diffs.push(d)
    }
  }







/**
 * Returns an empty node state.
 */
function init() {
    return Map({opSet: OpSet.init()})
}


/**
 * Constructs a patch object from the current node state `state` and the list
 * of object modifications `diffs`.
 */
function makePatch(state, diffs) {
    const canUndo = state.getIn(['opSet', 'undoPos']) > 0
    const canRedo = !state.getIn(['opSet', 'redoStack']).isEmpty()
    const clock = state.getIn(['opSet', 'clock']).toJS()
    const deps = state.getIn(['opSet', 'deps']).toJS()
    return {clock, deps, canUndo, canRedo, diffs}
}

/**
 * Returns a patch that, when applied to an empty document, constructs the
 * document tree in the state described by the node state `state`.
 */
function getPatch(state) {
    let diffs = [], opSet = state.get('opSet')
    let context = new MaterializationContext(opSet)
    context.instantiateObject(opSet, OpSet.ROOT_ID)
    context.makePatch(OpSet.ROOT_ID, diffs)
    return makePatch(state, diffs)
}

function getChanges(oldState, newState) {
    const oldClock = oldState.getIn(['opSet', 'clock'])
    const newClock = newState.getIn(['opSet', 'clock'])
    if (!lessOrEqual(oldClock, newClock))
        throw new RangeError('Cannot diff two states that have diverged')
    return OpSet.getMissingChanges(newState.get('opSet'), oldClock).toJS()
}

function getChangesForActor(state, actorId) {
    // I might want to validate the actorId here
    return OpSet.getChangesForActor(state.get('opSet'), actorId).toJS()
}

function getMissingChanges(state, clock) {
    return OpSet.getMissingChanges(state.get('opSet'), clock).toJS()
}
  
  function getMissingDeps(state) {
    return OpSet.getMissingDeps(state.get('opSet'))
  }


/**
 * Applies a list of `changes` from remote nodes to the node state `state`.
 * Returns a two-element array `[state, patch]` where `state` is the updated
 * node state, and `patch` describes the modifications that need to be made
 * to the document objects to reflect these changes.
 */
function applyChanges(state, changes) {
    return apply(state, changes, false)
  }


/**
 * Takes a single change request `change` made by the local user, and applies
 * it to the node state `state`. The difference to `applyChange()` is that this
 * function adds the change to the undo history, so it can be undone (whereas
 * remote changes are not normally added to the undo history). Returns a
 * two-element array `[state, patch]` where `state` is the updated node state,
 * and `patch` confirms the modifications to the document objects.
 */
function applyLocalChange(state, change) {
    if (typeof change.actor !== 'string' || typeof change.seq !== 'number') {
      throw new TypeError('Change request requries `actor` and `seq` properties')
    }
    // Throw error if we have already applied this change request
    if (change.seq <= state.getIn(['opSet', 'clock', change.actor], 0)) {
      throw new RangeError('Change request has already been applied')
    }
  
    let patch
    if (change.requestType === 'change') {
      ;[state, patch] = apply(state, [change], true)
    } else if (change.requestType === 'undo') {
      ;[state, patch] = undo(state, change)
    } else if (change.requestType === 'redo') {
      ;[state, patch] = redo(state, change)
    } else {
      throw new RangeError(`Unknown requestType: ${change.requestType}`)
    }
    patch.actor = change.actor
    patch.seq   = change.seq
    return [state, patch]
  }

/**
 * The implementation behind `applyChanges()` and `applyLocalChange()`.
 */
function apply(state, changes, undoable) {
    let diffs = [], opSet = state.get('opSet')
    for (let change of fromJS(changes)) {
      change = change.remove('requestType')
      const [newOpSet, diff] = OpSet.addChange(opSet, change, undoable)
      for (let d of diff) diffs.push(d)
      opSet = newOpSet
    }
  
    state = state.set('opSet', opSet)
    return [state, makePatch(state, diffs)]
  }
