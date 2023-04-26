package com.bs.glr.service.impl;

import com.bs.glr.bean.BsBookInfo;
import com.bs.glr.bean.BsBookInfoExample;
import com.bs.glr.mapper.BsBookInfoMapper;
import com.bs.glr.service.BookService;
import org.springframework.stereotype.Service;

import javax.annotation.Resource;
import java.util.List;

/**
 * @作者 GLQ
 * @时间 2022-03-31 22:51
 * @描述 ${var}
 */
@Service
public class BookServiceImpl implements BookService {

    @Resource
    BsBookInfoMapper bookInfoMapper;

    @Override
    public Integer insertBookInfo(BsBookInfo bsBookInfo) {
        return bookInfoMapper.insertSelective(bsBookInfo);
    }

    @Override
    public BsBookInfo selectBookInfoByRfid(String rfid) {
        BsBookInfoExample example = new BsBookInfoExample();
        example.createCriteria().andBRfidEqualTo(rfid);
        List<BsBookInfo> bsBookInfos = bookInfoMapper.selectByExample(example);
        if(bsBookInfos== null || bsBookInfos.size()<=0){
            return null;
        }
        return bsBookInfos.get(0);
    }

    @Override
    public Integer updateBookInfoByKey(BsBookInfo bsBookInfo1) {
        return bookInfoMapper.updateByPrimaryKeySelective(bsBookInfo1);
    }
}
