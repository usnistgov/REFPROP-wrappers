Rem
Rem Copy the function list to the Mathcad installation \doc\func directory
REm
xcopy "..\doc\funcdoc\Refprop_EN.xml" "C:\Program Files (x86)\Mathcad\Mathcad 15\doc\funcdoc" /rqky
Rem 
Rem Copy the Refprop html help file to the Mathcad installation \doc\HELP_EN directory
Rem
xcopy "..\doc\HELP_EN\Refprop" "C:\Program Files (x86)\Mathcad\Mathcad 15\doc\HELP_EN\Refprop" /risqky
Rem
Rem Copy the Refprop Handbook master file to the Mathcad installation \Handbook directory
Rem
xcopy "..\Handbook\Refprop.hbk" "C:\Program Files (x86)\Mathcad\Mathcad 15\Handbook" /rqky
Rem
Rem Copy the Refprop Handbook worksheet files to the Mathcad installation \Handbook directory
Rem
xcopy "..\Handbook\Refprop" "C:\Program Files (x86)\Mathcad\Mathcad 15\Handbook\Refprop" /risqky