NDN tester
=====================

This is an application which had been developed to test the NDN Zrt.'s test webservices.

Features:
 - Syncronize the items and the related data from the NDN to a local SQLITE database
 - Create and upload a stock and sales report
 - Retrive these reports through the monitoring interface

Dependencies:
 - Qt 5 (tested with Qt 5.0.1)
 - KDSOAP (https://github.com/KDAB/KDSoap)
 - QCA (Qt Cryptographics Architecture: git://anongit.kde.org/qca.git) 
 


----------

Ez egy tesztalkalmazás amely segítségével elérhető az NDN Zrt teszt webservicei amelyeken keresztül tesztelhetőek a [minősített készletnyilvántartó rendszereknek][1] .

Főbb képességek:
 - Törzsadatok leszinkronizálása egy SQLITE adatbázisba
 - Napi nyitókészlet, forgalmi adatok feltöltése illetve azok visszaellenőrzése grafikus felületen.

Függőségek:
 - Qt 5 (Qt 5.0.1-el tesztelve)
 - KDSOAP (https://github.com/KDAB/KDSoap)
 - QCA (Qt Cryptographics Architecture: git://anongit.kde.org/qca.git) 
 


  [1]: http://nemzetidohany.hu/adatszolgaltatas/az-nd-nonprofit-zrt-altal-minositett-keszletnyilvantarto-kiskereskedelmi-rendszerek/