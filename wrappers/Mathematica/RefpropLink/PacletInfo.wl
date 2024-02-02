(* ::Package:: *)

PacletObject[
    <|
        "Name" -> "RefpropLink",
        "Version" -> "1.1.0",
        "WolframVersion" -> "13.0+",
        "SystemID" -> {"Windows-x86-64"},
        "Description" -> "Provides wrapper functions for the NIST REFPROP materials library functions.",
        "Creator" -> "J. P. Henning",
        "URL" -> "https://github.com/usnistgov/REFPROP-wrappers",
        
        "Extensions" ->
            {
                {
                    "Kernel",
                    "Root" -> "Kernel",
                    "Context" -> "RefpropLink`"
                },
                {
                    "Documentation",
                    "Language" -> "English",
                    "MainPage" -> "Tutorials/RefpropLinkOverview"
                },
                {"FrontEnd"},
                {
                    "Resources",
                    "Root" -> "TextREsources",
                    "Resources" -> {{"FilePaths","Filepaths.txt"}}
                }
            }
    |>
]
