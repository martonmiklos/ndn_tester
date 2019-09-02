#!/bin/bash

declare -a arr=('Monitoring' 'MasterData' 'Transactions')
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"


if [ $# -ne 3 ]; then
    echo "Usage: generate.sh KDWSDL_EXECUTABLE PKCS12_cert_path cert_password"
else 
    rm -rf $DIR/*.cpp $DIR/*.h
    for wsdl in "${arr[@]}"
    do
        eval $1 -both $DIR/$wsdl https://adatkuldes-teszt.nemzetidohany.gov.hu:8444/ndc/$wsdl.svc?wsdl -pkcs12file $2 -pkcs12password $3 -namespace NS$wsdl -namespaceMapping @$DIR/namespacemapping.txt
    done
fi
