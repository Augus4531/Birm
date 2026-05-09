# 安装openblas
## 1、Ubuntu/Debian
sudo apt install gcc gfortran make

## RHEL/CentOS
sudo yum install gcc-gfortran make

## 2、解压
unzip OpenBLAS-develop.zip

## 3、进入目录
cd OpenBLAS-develop

## 4、Ubuntu/Debian 系统
sudo apt update
sudo apt install gcc gfortran make -y

## CentOS/RHEL 系统
sudo yum install gcc-gfortran make -y

## 5、自动检测 CPU 架构并编译
make -j$(nproc)
在test文件报错没事
## 6、安装
sudo make PREFIX=/opt/OpenBLAS install

## 7、配置环境
echo 'export LD_LIBRARY_PATH=/opt/OpenBLAS/lib:$LD_LIBRARY_PATH' >> ~/.bashrc
source ~/.bashrc