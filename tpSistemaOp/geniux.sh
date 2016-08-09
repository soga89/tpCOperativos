# Editar bashrc
#echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/TPOperativos/delEquipo/Debug" >> .bashrc
#echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/TPOperativos/commons/Debug" >> .bashrc

# Condiciones para las carpetas
# Si existen las borra
if [ -d "/home/utnso/TPOperativos/bin" ] ; then rm -rf /home/utnso/TPOperativos/bin ; fi
if [ -d "/home/utnso/TPOperativos/logs" ] ; then rm -rf /home/utnso/TPOperativos/logs ; fi
if [ -d "/home/utnso/TPOperativos/test" ] ; then rm -rf /home/utnso/TPOperativos/test ; fi

mkdir /home/utnso/TPOperativos/bin
mkdir /home/utnso/TPOperativos/logs
mkdir /home/utnso/TPOperativos/test

# Make de todos los procesos
(cd /home/utnso/TPOperativos/commons && sudo make install ) > make.dat
(cd /home/utnso/TPOperativos/delEquipo/Debug/ && sudo make all ) >make.dat
(cd /home/utnso/TPOperativos/cpu/Debug/ && make all ) > make.dat
(cd /home/utnso/TPOperativos/memoria/Debug/ && make all ) > make.dat
(cd /home/utnso/TPOperativos/swap/Debug/ && make all ) > make.dat
(cd /home/utnso/TPOperativos/planificador/Debug/ && make all) >  make.dat
rm make.dat

# Copiar los ejecutables en ./bin
cp /home/utnso/TPOperativos/cpu/Debug/cpu /home/utnso/TPOperativos/bin/cpu
cp /home/utnso/TPOperativos/memoria/Debug/memoria /home/utnso/TPOperativos/bin/memoria
cp /home/utnso/TPOperativos/swap/Debug/swap /home/utnso/TPOperativos/bin/swap
cp /home/utnso/TPOperativos/planificador/Debug/planificador /home/utnso/TPOperativos/bin/planificador

# Copiar los test en ./test
cp /home/utnso/TPOperativos/cpu/Debug/testCPU /home/utnso/TPOperativos/test/testCPU
cp /home/utnso/TPOperativos/memoria/Debug/testMemoria /home/utnso/TPOperativos/test/testMemoria
cp /home/utnso/TPOperativos/swap/Debug/testSwap /home/utnso/TPOperativos/test/testSwap
cp /home/utnso/TPOperativos/planificador/Debug/testPlanificador /home/utnso/TPOperativos/test/testPlanificador