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
