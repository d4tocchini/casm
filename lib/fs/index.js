
const {
    readFile,
    readFileSync,
    existsSync,
    mkdirSync,
    promises: {
        open,
        unlink,
    }
} = require('fs');

module.exports = {
    open, // (path, flags[, mode]) => filehandle
    rm,
    read_file,
    read_utf8,
    read_buffer,
    read_utf8_sync,
    read_buffer_sync,
    read_file_sync,
    mkdir_sync,
    exists_sync
}

const read_utf8_opts = {encoding:'utf8',flag:'r'};
const read_buffer_opts = {encoding:undefined,flag:'r'};
// https://nodejs.org/dist/latest-v12.x/docs/api/buffer.html#buffer_buffers_and_character_encodings
const read_encoding = {
    'utf8': read_utf8_opts,
    'buffer':read_buffer_opts,
    // 'base64'
    // 'binary' - Alias for 'latin1'
    // 'hex'
}
function read_utf8_sync(path) { return read_file_sync(path,read_utf8_opts) }
function read_buffer_sync(path) { return read_file_sync(path,read_utf8_opts) }
function read_file_sync(path, encoding) {
    return readFileSync(path, read_encoding[encoding]);
}
let _read_path = '';
let _read_opts = read_utf8_opts;
function read_file(path, encoding) {
    _read_path = path;
    _read_opts = read_encoding[encoding];
    return new Promise(_read_promise);
}
function read_utf8(path) {
    _read_path = path;
    _read_opts = read_utf8_opts;
    return new Promise(_read_promise);
}
function read_buffer(path) {
    _read_path = path;
    _read_opts = read_buffer_opts;
    return new Promise(_read_promise);
}
function _read_promise(resolve, reject) {
    readFile(_read_path, _read_opts, function(err,data) {
        if (err)
            reject(err)
        else
            resolve(data)
    })
}

async function rm ( // resolves 1 upon success, 0 upon failure
                path) // <string> | <Buffer> | <URL>
{
                let status = 1;
                try { await unlink(path); }
                catch (e) { status = 0; }
                return status;
}

const       mkdir_sync_opts = { recursive: true }
function    mkdir_sync(path) { mkdirSync(path, mkdir_sync_opts);}
function    exists_sync(path) { return existsSync(path);}