'use strict';

const install = require('addon-tools-raub/install');


const prefix = 'https://github.com/node-3d/deps-qmlui-raub/releases/download';
const tag    = process.env.npm_package_config_install;

install(`${prefix}/${tag}`);

'use strict';

const { install } = require('addon-tools-raub');


const prefix = 'https://github.com/node-3d/deps-qt-gui-raub/releases/download';
const tag = '3.0.0';

install(`${prefix}/${tag}`);
