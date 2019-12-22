const app = getApp()
Page({
  data: {
    inputValue: "",//接受输入的all，可拓展为根据日期删除数据
    outvalue: "",
    count: 1,
    cleardata: "",
    df1: "",
  },
  /**
   * 请求信息制作
   */
  makeObj : function(){

    var obj = {
      url: "http://api.heclouds.com/devices/561746562/datastreams/value",//删除的是数据流value（慎用）
 
      header: {
        "api-key": "SXop0l4xynA9GrZFJNKPB0Vu3=g=",
      },
      method: "DELETE",
      
      success: function (res) {
        console.log(res)
        wx.showToast({
          title: '清除成功',
          icon: 'success',
          duration: 2000//持续的时间
        })//用户提醒
      }
    }
    return obj;
  },
  /**
   * 发送请求
   */
  sendRequset: function (obj) {
    wx.request(obj);
  },
  saveDataToServer: function (event) {

    this.sendRequset(this.makeObj())

  },
  /**
   * 动态改变全局变量实现分组
   */
  df: function (e) {
    var value = e.detail.value
    app.globalData.dataflag=value
    //app.globalData.dataflag= df1
     
     /* wx.showToast({
        title: '加入成功',
        icon: 'success',
        duration: 2000//持续的时间
        })*/
  },
  /**
   * 成功提示
   */
  tishi: function(){
    wx.showToast({
      title: '加入成功',
      icon: 'success',
      duration: 2000//持续的时间
    })
  },
  /**
   * 清空数据库
   */
  clear: function () {
    var cleardata = this.data.cleardata
    //console.log(cleardata)
    wx.cloud.callFunction({
      name: "fdeletedata",
      data: {
        type: cleardata,
        // yuefen:cleardata
      },
      success: function (res) {
        console.log(res)
        wx.showToast({
          title: '删除成功',
          icon: 'success',
          duration: 2000//持续的时间
        })
      }
    })

  },
  /**
   * 读取数据（拓展功能此处未用）
   */
  readdata: function () {
    var _this = this
    wx.cloud.callFunction({
      name: 'newsearch',
      data: {
        tag: 2//后续可更换为一个变量
      },
      success: function (res) {
        var result = res.result.data[res.result.data.length - 1]
        console.log(result)
        _this.setData({
          outvalue: result.input
        })
      },
      fail: console.error
    })
  },
  /**
   * 获得输入框数据
   */
  input: function (e) {
    var value = e.detail.value
    this.setData({
      inputValue: value
    })
  },
  /**
   * 获得输入框数据
   */
  input1: function (e) {
    var value = e.detail.value
    this.setData({
      cleardata: value
    })
  },
  /**
   * 上传（此处未用）
   */
  upload: function () {
    var m = " "
    var input = this.data.inputValue
    var a = this.data.count++  //区分条数，依次累加
    var db = wx.cloud.database()
    db.collection("final").add({
      data: {
        input: input,
        tag: a,
       // yuefen: '2019.11'  拓展功能
      },
      success: function (res) {
        console.log(res)
        wx.showToast({
          title: '发送成功',
          icon: 'success',
          duration: 2000//持续的时间
        })
      }
    })
  },

})
 