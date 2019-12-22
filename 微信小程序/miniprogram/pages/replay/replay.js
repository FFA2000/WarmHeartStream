var that
const db = wx.cloud.database();
Page({

  /**
   * 页面的初始数据
   */
  data: {
    id: '',
    openid: '',
    content: '',
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function(options) {
    that = this;
    that.data.id = options.id;
    that.data.openid = options.openid;
  },

  bindKeyInput(e) {
    that.data.content = e.detail.value;
    console.log("内容：" + that.data.content)

  },

  play: function () {
    //播放声音文件      
    wx.playVoice({
      filePath: that.data.voice
    })
  },

  start: function () {
    //开始录音      
    wx.startRecord({
      success: function (e) {
        that.setData({
          voice: e.tempFilePath,
        })
      }
    })
  },

  stop: function () {
    //结束录音      
    wx.stopRecord();
  },

  saveReplay: function() {
    db.collection('replay').add({
      // data 字段表示需新增的 JSON 数据
      data: {
        content: that.data.content,
        voice:that.data.voice,
        date: new Date(),
        r_id: that.data.id,
        u_id: that.data.openid,
        t_id: that.data.id,
      },
      success: function(res) {
        wx.showToast({//提示
          title: '发布成功',
        })
        setTimeout(function() {
          wx.navigateBack({
            url: "../homeDetail/homeDetail?id=" + that.data.id + "&openid=" + that.data.openid
          })
        }, 1500)

      },
      fail: console.error
    })
  }

})