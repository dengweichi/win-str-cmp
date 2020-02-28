{
"targets": [
    {
    "target_name": "winStrCmp",
    "sources": [ "src/main.cpp" ],
      "conditions:": [
         ["OS=='win'",{
           "libraries": ["-lnode.lib"]
         }]
      ]
    },
  ]
}
