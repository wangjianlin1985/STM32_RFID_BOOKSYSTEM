package com.bs.glr.controller;

import com.alibaba.druid.util.StringUtils;
import com.bs.glr.bean.BsUser;
import com.bs.glr.bean.ResultBean;
import com.bs.glr.service.UserService;
import com.github.pagehelper.PageInfo;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.redis.core.StringRedisTemplate;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import javax.annotation.Resource;
import java.util.List;
import java.util.UUID;

/**
 * @author glr
 * @create 2021-01-31 14:08
 * @desc ${var}
 */
@RestController
public class UserController {
    @Resource
    UserService userService;
    @Autowired
    private StringRedisTemplate redisTemplate;

    @RequestMapping("/user/login")
    public ResultBean login(String uname, String pwd) {

        if (StringUtils.isEmpty(uname) || StringUtils.isEmpty(pwd)) {
            return ResultBean.returnError().pushMessage("请将用户名和密码输入完整");
        }

        List<BsUser> userList = userService.getUserByUname(uname);
        if (userList == null || userList.size() == 0) {
            return ResultBean.returnError().pushMessage("用户不存在");
        }


        if (!userList.get(0).getPwd().equals(pwd)) {
            return ResultBean.returnError().pushMessage("用户名或密码不正确");
        }
        String uuid = UUID.randomUUID().toString();


//        redisTemplate.opsForValue().set("user_token:" + uuid, uname);
        userService.loginToken(uuid, userList.get(0).getId());

        return ResultBean.returnOk().pushData("token", uuid);
    }


    @RequestMapping("/user/webRegister")
    public ResultBean register(String uname, String pwd) {

        //校验用户名是否存在
        List<BsUser> userByUname = userService.getUserByUname(uname);
        if (userByUname != null && userByUname.size() > 0) {
            return ResultBean.returnError().pushMessage("用户名已经存在，请重新输入");
        }
        //插入用户表
        Integer uid = userService.insertUser(uname,pwd);
        //插入token表
        String token = UUID.randomUUID().toString();
        userService.insertToken(uid,token);

        return ResultBean.returnOk().pushData("token", token).pushMessage("注册成功");
    }


    //用户查询
    @RequestMapping("/auth/selectUserList")
    public ResultBean selectUserList(Integer page,Integer limit) {

        if(page == null ){
            page = 1;
        }
        if(limit == null){
            limit =10;
        }

        PageInfo<BsUser> pageInfo = userService.selectUserListByPage(page,limit);
        long total = pageInfo.getTotal();
        List<BsUser> list = pageInfo.getList();

        return ResultBean.returnOk().pushData("total", total).pushData("list",list);
    }


    //用户删除
    @RequestMapping("/auth/deleteUser")
    public ResultBean deleteUser(Integer delUid) {

        userService.deleteUserById(delUid);

        return ResultBean.returnOk().pushMessage("删除成功");
    }

    //修改用户密码
    @RequestMapping("/auth/updatePwdByUid")
    public ResultBean updatePwdByUid(Integer uid,String pwd) {

        BsUser bsUser = userService.selectUserByUid(uid);
        if(bsUser == null){
            return ResultBean.returnError().pushMessage("用户不存在");
        }
        userService.updatePwdByUid(uid,pwd);

        return ResultBean.returnOk().pushMessage("修改成功");
    }

    /**
     * @Author L H
     * @desc //获取信息
     * @Date  2021/2/20
     **/
    @RequestMapping("/auth/info")
    public ResultBean changePwd (String token){
        BsUser user = userService.getUserByToken(token);
        if(user == null){
            return ResultBean.returnError("用户不存在");
        }

        String role = "";

        if("0".equals(user.getIdentity())){
            role = "man";
        }else {
            role = "pan";
        }

        if(user!=null){
            return ResultBean.returnOk().pushData("name",user.getUname()).pushData("avatar","https://scwtest20200706.oss-cn-beijing.aliyuncs.com/jd.jpg")
                    .pushData("roles",role);
        }
        return ResultBean.returnError();
    }


}
