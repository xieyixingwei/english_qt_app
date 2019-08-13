The new generation of LTE mPCIE modems provide both HSDPA/3G and LTE/4G wireless connectivity, these modems by default use the newer QMI interface as the primary connection method, as opposed to PPP over serial or Direct IP as with earlier modems in the range, and are currently best supported by Raspbian Jessie OS or above as this has support for the libqmi interface tools.  
新一代LTE mPCIE调制解调器提供HSDPA/3G和LTE/4G无线连接，默认情况下这些调制解调器使用较新的QMI接口作为主要连接方法，而不是PPP串口或直接IP，与早期调制解调器一样 ，目前最好由Raspbian Jessie OS或更高版本支持，因为它支持libqmi接口工具。  

In this example we'll use a Sierra Wireless MC7304 modem and start by installing the udev rule (20-modem-7304.rules) in /etc/udev/rules.d from the modem rule pack.  
在这个例子中，我们将使用Sierra Wireless MC7304调制解调器，首先在调制解调器规则包中的/etc/udev/rules.d中安装udev规则（20-modem-7304.rules）。  

This sets up shortcut references to allow us to consistently identify which of the ttyUSBx ports do which functions.  
这将设置快捷方式引用，以允许我们一致地识别哪个ttyUSBx端口执行哪些功能。  

Whilst some LTE modems still provide a legacy serial port for PPP style connection modem control is optimally done through the /dev/cdc-wdm0 QMI interface so we need to install the software support packages via the command below:  
虽然一些LTE调制解调器仍然提供用于PPP样式连接的传统串行端口调制解调器控制是通过/ dev / cdc-wdm0 QMI接口最佳完成的，因此我们需要通过以下命令安装软件支持包：  
```
apt-get install libqmi-utils
```

This installs two main utilities (qmi-cli tool and qmi-network helper script) these are used for interaction with the modem (see man qmi-cli for more details)  
这将安装两个主要实用程序（qmi-cli工具和qmi-network帮助程序脚本），这些实用程序用于与调制解调器交互（有关详细信息，请参阅man qmi-cli）  

The APN setting is set by creating and then adding a line to /etc/qmi-network.conf - Consult your carrier for the correct APN name and insert it into the configuration file as shown below :  
通过创建然后在/etc/qmi-network.conf中添加一行来设置APN设置 - 请咨询运营商以获取正确的APN名称，并将其插入配置文件中，如下所示：  
```
echo "APN=internet" >/etc/qmi-network.conf
```

I found the the next step provided easier control over the Sierra Wireless modem and routing by first removing the extra unused network ports, but this step is optional and can be skipped on other modem models:  
我发现下一步通过首先删除额外的未使用的网络端口，更容易控制Sierra Wireless调制解调器和路由，但此步骤是可选的，可以在其他调制解调器型号上跳过：  

