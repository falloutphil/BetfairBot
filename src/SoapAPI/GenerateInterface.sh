# Proxy Generation
rm -f *.h *.cpp *.nsmap
wsdl2h -o Betfair.h https://api.betfair.com/global/v3/BFGlobalService.wsdl https://api.betfair.com/exchange/v5/BFExchangeService.wsdl
soapcpp2 -x -i -C -I /usr/local/share/gsoap/import Betfair.h

#Namespace fudge
rm -f BFExchangeServices.nsmap
mv -f BFGloabalService.nsmap BetFair.nsmap



