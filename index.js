'use strict';

const tools = require('addon-tools-raub');
const qml = require('deps-qt-qml-raub');


module.exports = {
	qml,
	gui  : qml.gui,
	core : qml.core,
	...tools.paths(__dirname),
};
