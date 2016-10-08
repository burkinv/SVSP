REM makecert -n "CN=Foiter Labs(c) GmBH Root CA" -r -a sha1 -sv FoiterLabsGmbhRootCA.pvk FoiterLabsGmbhRootCA.cer -sr LocalMachine -ss "Foiter Labs(c)" -sky signature
REM makecert -sv FoiterLabsGmbhCA.pvk -iv FoiterLabsGmbhRootCA.pvk -n "CN=Foiter Labs(c) GmBH" -ic FoiterLabsGmbhRootCA.cer FoiterLabsGmbhCA.cer
REM pvk2pfx -pvk FoiterLabsGmbhCA.pvk -spc FoiterLabsGmbhCA.cer -pfx FoiterLabsGmbhCA.pfx -po foiter

signtool sign /f FoiterLabsGmbhCA.pfx /p foiter /v /t "http://timestamp.verisign.com/scripts/timestamp.dll" AVIONICS.dll