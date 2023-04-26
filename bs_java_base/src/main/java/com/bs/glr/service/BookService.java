package com.bs.glr.service;

import com.bs.glr.bean.BsBookInfo; /**
 * @作者 GLQ
 * @时间 2022-03-31 22:50
 * @描述 ${var}
 */
public interface BookService {
    Integer insertBookInfo(BsBookInfo bsBookInfo);

    BsBookInfo selectBookInfoByRfid(String rfid);

    Integer updateBookInfoByKey(BsBookInfo bsBookInfo1);
}
