{
"targets": [
    {
    "target_name": "winStrCmp",
    "sources": [ "src/main.cpp" ],
	"defines":["DEBUGGER"],
    "conditions:": [
         ["OS=='win'",{
           "libraries": ["-lnode.lib"],
		   "defines":"WIN"
         }]
	    ],
    },
  ]
}
