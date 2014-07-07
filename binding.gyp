{
  "targets": [
    {
      "target_name": "EventReader",
      "sources": [ "src/EventReader.cpp" ],
	  'conditions': [
          ['OS=="win"', {
            'msvs_settings': {
              'VCCLCompilerTool': {                  
                'RuntimeTypeInfo':'true',
                'RuntimeLibrary':'MultiThreadedDLL'
              }
            },
            'msbuild_settings': {
              'ClCompile': {
                'ExceptionHandling': 'Async',
                'CompileAsManaged':'true'
              } 
            },
			'libraries' : ['advapi32.lib']
          }]
      ],
	  "include_dirs": ["src/","dll/"]
    }
  ]
}