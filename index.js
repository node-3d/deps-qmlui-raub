'use strict';

const qml = require('deps-qt-qml-raub');


module.exports = {
	qml,
	gui  : qml.gui,
	core : qml.core,
	...require('addon-tools-raub').getPaths(__dirname),
};
