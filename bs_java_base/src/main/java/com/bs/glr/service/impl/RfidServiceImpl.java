package com.bs.glr.service.impl;

import com.bs.glr.bean.BsRfid;
import com.bs.glr.mapper.BsRfidMapper;
import com.bs.glr.service.RfidService;
import org.springframework.stereotype.Service;

import javax.annotation.Resource;

/**
 * @作者 GLQ
 * @时间 2022-02-28 17:56
 * @描述 ${var}
 */
@Service
public class RfidServiceImpl implements RfidService {

    @Resource
    BsRfidMapper rfidMapper;

    @Override
    public Integer updateStatus(String s) {
        BsRfid bsRfid = new BsRfid();
        bsRfid.setRfidStatus(s);
        bsRfid.setId(1);
        return rfidMapper.updateByPrimaryKeySelective(bsRfid);
    }

    @Override
    public BsRfid getRfidId() {
        return rfidMapper.selectByPrimaryKey(1);
    }

    @Override
    public Integer insertRfid(String rfidId) {
        BsRfid bsRfid = new BsRfid();
        bsRfid.setId(1);
        bsRfid.setRfidStatus("0");
        bsRfid.setRfidId(rfidId);
        return rfidMapper.updateByPrimaryKeySelective(bsRfid);
    }

    @Override
    public void setRfidStatusToOne() {
        BsRfid bsRfid = new BsRfid();
        bsRfid.setId(1);
        bsRfid.setRfidStatus("1");
        rfidMapper.updateByPrimaryKeySelective(bsRfid);
    }
}
