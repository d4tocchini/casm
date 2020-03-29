const SHMap = require("../index.js")

TEST("",function (Y) {
    const map = new SHMap("")
    Y(!map.needs_save());
    map.set("name", "bob");
    Y(map.needs_save());
    map.save();
    Y(!map.needs_save());
    map.free();
})

TEST("",function (Y) {
    const map = shmap.init("");
    Y(!map.needs_save());
    const name = map.get("name");
    Y(name === "bob")
})