# 暖心弹幕
## 安装说明

0. 在微信小程序客户端创建一个云开发项目
1. 在云开发控制台页面，选择数据库，创建 `topic`,`collect`,`history`,`replay` 四个集合
2. 下载到本地 git clone https://github.com/FFA2000/WarmHeartStream.git 或者 下载 zip
3. 将 miniprogram 目录下的文件全部复制到你的 miniprogram
4. 修改 `app.js` 中的 `globalData` 字段修改
	```
	this.globalData = {
      openid: '你的openid',
      evn: '你的开发环境'
    }
    ```
