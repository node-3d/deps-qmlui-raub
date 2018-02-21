{
	'variables': {
		'cp'    : '<!(node -e "require(\'addon-tools-raub\').cp()")',
		'mkdir' : '<!(node -e "require(\'addon-tools-raub\').mkdir()")',
		'rm'    : '<!(node -e "require(\'addon-tools-raub\').rm()")',
		'rem'   : '<!(node -e "require(\'.\').rem()")',
	},
	'targets': [
		{
			'target_name' : 'remove_extras',
			'type'        : 'none',
			'actions'     : [
				{
					'action_name' : 'Unnecessary binaries removed.',
					'inputs'      : [],
					'outputs'     : ['build'],
					'action'      : ['<(rm)', '-rf', '<@(rem)'],
				}
			],
		},
		{
			'target_name'  : 'make_directory',
			'type'         : 'none',
			'dependencies' : ['remove_extras'],
			'actions'      : [{
				'action_name' : 'Directory created.',
				'inputs'      : [],
				'outputs'     : ['qt'],
				'action': ['<(mkdir)', '-p', 'include']
			}],
		},
		{
			'target_name'  : 'copy_binary',
			'type'         : 'none',
			'dependencies' : ['make_directory'],
			'actions'      : [{
				'action_name' : 'Header copied.',
				'inputs'      : [],
				'outputs'     : ['include'],
				'action'      : ['<(cp)', 'qt/qml-ui.hpp', 'include/qml-ui.hpp'],
			}],
		},
	]
}
