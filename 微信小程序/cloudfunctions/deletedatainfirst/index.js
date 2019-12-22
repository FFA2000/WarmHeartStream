const cloud = require('wx-server-sdk')
cloud.init({
  env: cloud.DYNAMIC_CURRENT_ENV
})
const db = cloud.database()
exports.main = async (event, context) => {
  let {
    type,
    _id
  } = event
  try {
     
      return await db.collection('topic').where({
        _id: type
        //_id: "01ace4015dea7c9700f2376260598f2d"
      }).remove()
  

  } catch (e) {
    console.error(e)
  }
}
