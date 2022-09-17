# wlink_ups
# wlink in ayi9 IoT card

作者 wechat 13902904910

wlink 物联网协议

	现在 ayi9 物联网卡在 UPS 产品中很普遍。ayi9 不仅支持老式的 SNMP v1 v2
	还支持新式的 wlink 物联网协议.

	wlink 协议在 UDP 8100 和 TCP 8200 提供服务，你可以通过 wlink 协议查询设备数据和向
	设备发送控制命令。

	wlink TCP 协议见 wlink_TCP_UPS.pdf 文件 

代码

	wlink_scan 示范了如何查询局域网内的设备.

	wlink_tcp  示范了如何查询设备数据和向设备发送命令.

	示范代码可以在 linux & windows 下编译运行。

	
	
