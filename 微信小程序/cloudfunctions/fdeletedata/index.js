const cloud = require('wx-server-sdk')
cloud.init({
  env: cloud.DYNAMIC_CURRENT_ENV
})
const db = cloud.database()
exports.main = async (event, context) => {
  let {
    type,
    //flagdate
  } = event
  try {
    if (type == 'all') {
      const _ = db.command
      return await db.collection('topic').where({
        content: _.exists(true)  
      }).remove()
    } else {
      return await db.collection('topic').where({
        content: type
      }).remove()
    }

  }  catch (e) {
    console.error(e)
  }
}
