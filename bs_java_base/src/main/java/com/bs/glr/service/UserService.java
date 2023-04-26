package com.bs.glr.service;

import com.bs.glr.bean.BsUser;
import com.github.pagehelper.PageInfo;

import java.util.List;

/**
 * @author glr
 * @create 2021-01-31 14:09
 * @desc ${var}
 */
public interface UserService {


    List<BsUser> getUserByUname(String uname);

    void loginToken(String uuid,Integer uid);


    String getUserNameByToken(String token);

    Integer insertUser(String uname, String pwd);

    void insertToken(Integer uid, String token);

    PageInfo<BsUser> selectUserListByPage(Integer page, Integer limit);

    void deleteUserById(Integer uid);

    void updatePwdByUid(Integer uid, String pwd);

    BsUser selectUserByUid(Integer uid);

    BsUser getUserById(Integer uid);

    BsUser getUserByToken(String token);
}
