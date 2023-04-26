package com.bs.glr.service;

import com.bs.glr.bean.BsRfid;

/**
 * @作者 GLQ
 * @时间 2022-02-28 17:56
 * @描述 ${var}
 */
public interface RfidService {
    Integer updateStatus(String s);

    BsRfid getRfidId();

    Integer insertRfid(String rfidId);

    void setRfidStatusToOne();
}
