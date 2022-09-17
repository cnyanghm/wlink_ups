# wlink in ayi9 IoT card for UPS

作者 wechat 13902904910

有两种程序方式连接到 ayi9 UPS 物联网卡

	1. wlink tcp 方式
	2. http post 方式


1. wlink 物联网协议

	现在 ayi9 物联网卡在 UPS 产品中很普遍。ayi9 不仅支持老式的 SNMP v1 v2
	还支持新式的 wlink 物联网协议.

	wlink 协议在 UDP 8100 和 TCP 8200 提供服务，你可以通过 wlink 协议查询设备数据和向
	设备发送控制命令。

	wlink TCP 协议见 wlink_TCP_UPS.pdf 文件 

	示范代码

	wlink_scan 示范了如何查询局域网内的设备.

	wlink_tcp  示范了如何查询设备数据和向设备发送命令.

	示范代码可以在 linux & windows 下编译运行。
	
2. http post 方式

	ayi9 的物联网卡还支持 http post 方式。这是更加容易上手的方式。
	
	你可以把类似 wlink 的请求直接 POST 给板上 web server。 详细见文件 http_post.pdf
	

	
	
