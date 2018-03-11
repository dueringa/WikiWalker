# JSON format 1

## Unlimited

    {
       "limits" : {
          "links" : 500
       },
       "batchcomplete" : "",
       "query" : {
          "normalized" : [
             {
                "to" : "Heinrich Ambros Eckert",
                "from" : "Heinrich_Ambros_Eckert"
             }
          ],
          "pages" : {
             "8283280" : {
                "links" : [
                   {
                      "title" : "10. Februar",
                      "ns" : 0
                   },
                   {
                      "title" : "16. Oktober",
                      "ns" : 0
                   },
                   // .....
                ],
                "title" : "Heinrich Ambros Eckert",
                "ns" : 0,
                "pageid" : 8283280
             }
          }
       }
    }

## Limited

    {
       "query" : {
          "pages" : {
             "8283280" : {
                "links" : [
                   {
                      "ns" : 0,
                      "title" : "10. Februar"
                   },
                   {
                      "ns" : 0,
                      "title" : "16. Oktober"
                   }
                ],
                "ns" : 0,
                "title" : "Heinrich Ambros Eckert",
                "pageid" : 8283280
             }
          },
          "normalized" : [
             {
                "to" : "Heinrich Ambros Eckert",
                "from" : "Heinrich_Ambros_Eckert"
             }
          ]
       },
       "continue" : {
          "continue" : "||",
          "plcontinue" : "8283280|0|1807"
       }
    }

# JSON format 2

## Unlimited

    {
       "limits" : {
          "links" : 500
       },
       "query" : {
          "pages" : [
             {
                "title" : "Heinrich Ambros Eckert",
                "pageid" : 8283280,
                "links" : [
                   {
                      "ns" : 0,
                      "title" : "10. Februar"
                   },
                   {
                      "title" : "16. Oktober",
                      "ns" : 0
                   },
                   // ...
                ],
                "ns" : 0
             }
          ],
          "normalized" : [
             {
                "to" : "Heinrich Ambros Eckert",
                "from" : "Heinrich_Ambros_Eckert",
                "fromencoded" : false
             }
          ]
       },
       "batchcomplete" : true
    }

## Limited

    {
       "query" : {
          "normalized" : [
             {
                "fromencoded" : false,
                "from" : "Heinrich_Ambros_Eckert",
                "to" : "Heinrich Ambros Eckert"
             }
          ],
          "pages" : [
             {
                "title" : "Heinrich Ambros Eckert",
                "links" : [
                   {
                      "ns" : 0,
                      "title" : "10. Februar"
                   },
                   {
                      "ns" : 0,
                      "title" : "16. Oktober"
                   }
                ],
                "pageid" : 8283280,
                "ns" : 0
             }
          ]
       },
       "continue" : {
          "continue" : "||",
          "plcontinue" : "8283280|0|1807"
       }
    }

## Continue generator

/w/api.php?action=query&format=json&prop=links&titles=$whatever&generator=links&formatversion=2&plnamespace=0&pllimit=$whatever&gplnamespace=0

    {
        "continue": {
            "plcontinue": "19978|0|Fort_William_Henry_Harrison",
            "continue": "||"
        },
        "query": {
            "pages": [
                {
                ...
                },
                // contains all pages with pageid, ns and title, bzt without links
                {
                ...
                }
            ]
        }
    }

## multiple titles

    {
        "batchcomplete": true,
        "query": {
            "normalized": [
                {
                    "fromencoded": false,
                    "from": "Zanfina_Ismajli",
                    "to": "Zanfina Ismajli"
                },
                {
                    "fromencoded": false,
                    "from": "Kleite_(Tochter_des_Danaos)",
                    "to": "Kleite (Tochter des Danaos)"
                }
            ],
            "pages": [
                {
                    "pageid": 2834303,
                    "ns": 0,
                    "title": "Zanfina Ismajli",
                    "links": [
                        {
                            "ns": 0,
                            "title": "10. Mai"
                        },
                        {
                            "ns": 0,
                            "title": "1985"
                        },
                        // ...
                    ]
                },
                {
                    "pageid": 8086803,
                    "ns": 0,
                    "title": "Kleite (Tochter des Danaos)",
                    "links": [
                        {
                            "ns": 0,
                            "title": "Aigyptos"
                        },
                        {
                            "ns": 0,
                            "title": "Altgriechische Sprache"
                        }
                        // ...
                    ]
                }
            ]
        },
        "limits": {
            "links": 500
        }
    }

## missing articles

    {
        "query": {
            "pages": [
                {
                    "ns": 0,
                    "title": "Drahtsäge",
                    "missing": true
                }
                // ...
            ]
        }
    }
