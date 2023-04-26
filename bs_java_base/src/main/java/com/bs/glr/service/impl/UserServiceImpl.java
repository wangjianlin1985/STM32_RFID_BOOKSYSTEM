package com.bs.glr.service.impl;

import com.bs.glr.bean.BsToken;
import com.bs.glr.bean.BsTokenExample;
import com.bs.glr.bean.BsUser;
import com.bs.glr.bean.BsUserExample;
import com.bs.glr.mapper.BsTokenMapper;
import com.bs.glr.mapper.BsUserMapper;
import com.bs.glr.service.UserService;
import com.github.pagehelper.PageHelper;
import com.github.pagehelper.PageInfo;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import javax.annotation.Resource;
import java.util.List;

/**
 * @author glr
 * @create 2021-01-31 14:09
 * @desc ${var}
 */
@Service
public class UserServiceImpl implements UserService {
    @Resource
    BsUserMapper userMapper;

    @Autowired
    BsTokenMapper tokenMapper;



    @Override
    public List<BsUser> getUserByUname(String uname) {
        BsUserExample example = new BsUserExample();
        example.createCriteria().andUnameEqualTo(uname);
        List<BsUser> bsUsers = userMapper.selectByExample(example);
        return bsUsers;
    }

    @Override
    public void loginToken(String uuid,Integer uid) {
        BsToken token = new BsToken();
        token.setToken(uuid);
        BsTokenExample example = new BsTokenExample();
        example.createCriteria().andUidEqualTo(uid);
        tokenMapper.updateByExampleSelective(token,example);
    }



    @Override
    public String getUserNameByToken(String token) {
        BsTokenExample tokenExample = new BsTokenExample();
        tokenExample.createCriteria().andTokenEqualTo(token);
        List<BsToken> bsTokens = tokenMapper.selectByExample(tokenExample);
        if(bsTokens==null ||  bsTokens.size()<=0 ){
            return null;
        }
//        BsUserExample userExample = new BsUserExample();
//        userExample.createCriteria().andIdEqualTo(1);
        BsUser bsUsers = userMapper.selectByPrimaryKey(bsTokens.get(0).getUid());
        if(bsUsers == null ){
            return null;
        }

        return bsUsers.getUname();
    }

    @Override
    public Integer insertUser(String uname, String pwd) {

        BsUser user = new BsUser();
        user.setPwd(pwd);
        user.setUname(uname);
        user.setIdentity("1");
        userMapper.insertSelective(user);
        return user.getId();
    }

    @Override
    public void insertToken(Integer uid, String token) {
        BsToken bsToken = new BsToken();
        bsToken.setUid(uid);
        bsToken.setToken(token);
        tokenMapper.insertSelective(bsToken);
    }

    @Override
    public PageInfo<BsUser> selectUserListByPage(Integer page, Integer limit) {
        PageHelper.startPage(page,limit);
        BsUserExample example = new BsUserExample();
        example.createCriteria().andIdentityEqualTo("1");
        example.setOrderByClause("id desc");
        List<BsUser> bsUsers = userMapper.selectByExample(example);
        PageInfo<BsUser> pageInfo = new PageInfo<>(bsUsers);
        return pageInfo;
    }

    @Override
    public void deleteUserById(Integer uid) {
        userMapper.deleteByPrimaryKey(uid);
    }

    @Override
    public void updatePwdByUid(Integer uid, String pwd) {

        BsUser user = new BsUser();
        user.setId(uid);
        user.setPwd(pwd);
        userMapper.updateByPrimaryKeySelective(user);
    }

    @Override
    public BsUser selectUserByUid(Integer uid) {

        return userMapper.selectByPrimaryKey(uid);
    }

    @Override
    public BsUser getUserById(Integer uid) {
        return userMapper.selectByPrimaryKey(uid);
    }

    @Override
    public BsUser getUserByToken(String token) {
        BsTokenExample example = new BsTokenExample();
        example.createCriteria().andTokenEqualTo(token);
        List<BsToken> bsTokens = tokenMapper.selectByExample(example);
        if(bsTokens == null || bsTokens.size()<=0){
            return null;
        }

        BsUser userById = getUserById(bsTokens.get(0).getId());
        return userById;
    }
}
