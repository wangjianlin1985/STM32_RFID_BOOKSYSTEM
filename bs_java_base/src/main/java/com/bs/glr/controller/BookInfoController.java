package com.bs.glr.controller;

import com.bs.glr.bean.BsBookInfo;
import com.bs.glr.bean.ResultBean;
import com.bs.glr.service.BookService;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import javax.annotation.Resource;
import java.math.BigDecimal;
import java.util.ArrayList;

/**
 * @作者 GLQ
 * @时间 2022-03-31 22:51
 * @描述 ${var}
 */
@RestController
@RequestMapping("/auth/book")
public class BookInfoController {
    @Resource
    BookService bookService;

    @RequestMapping("addBook")
    public ResultBean addBook (String bname, String bmsg, BigDecimal bprice,Integer bnumber,String brfid){
        BsBookInfo bsBookInfo1 = bookService.selectBookInfoByRfid(brfid);
        if(bsBookInfo1!= null){
            return ResultBean.returnError("该IC卡已经绑定书籍，不可重复绑定");
        }

        BsBookInfo bsBookInfo = new BsBookInfo();
        bsBookInfo.setbMsg(bmsg);
        bsBookInfo.setbName(bname);
        bsBookInfo.setbPrice(bprice);
        bsBookInfo.setbNumber(0);
        bsBookInfo.setbRfid(brfid);

        Integer line = bookService.insertBookInfo(bsBookInfo);
        if(line>0){
            return ResultBean.returnOk();
        }
        return ResultBean.returnError();

    }


    @RequestMapping("selectBookByRfid")
    public ResultBean selectBookByRfid (String rfid){

        BsBookInfo bsBookInfo = bookService.selectBookInfoByRfid(rfid);
        if(bsBookInfo == null){
            return ResultBean.returnError("未获取到书籍信息");
        }

        ArrayList<BsBookInfo> list = new ArrayList<>();

        list.add(bsBookInfo);
        return ResultBean.returnOk().pushData("data",list);
    }

    //书籍入库模块
    @RequestMapping("bookCheckOut")
    public ResultBean bookCheckOut (String rfid){

        BsBookInfo bsBookInfo = bookService.selectBookInfoByRfid(rfid);
        if(bsBookInfo == null){
            return ResultBean.returnError("未获取到书籍信息");
        }

        Integer integer = bsBookInfo.getbNumber();
        integer++;
        BsBookInfo bsBookInfo1 = new BsBookInfo();
        bsBookInfo1.setId(bsBookInfo.getId());
        bsBookInfo1.setbNumber(integer);
        Integer line = bookService.updateBookInfoByKey(bsBookInfo1);
        if(line>0){
            return ResultBean.returnOk();
        }
        return ResultBean.returnError();
    }

    //书籍出库模块
    @RequestMapping("bookPut")
    public ResultBean bookPut (String rfid){
        BsBookInfo bsBookInfo = bookService.selectBookInfoByRfid(rfid);
        if(bsBookInfo == null){
            return ResultBean.returnError("未获取到书籍信息");
        }

        Integer integer = bsBookInfo.getbNumber();
        integer--;
        BsBookInfo bsBookInfo1 = new BsBookInfo();
        bsBookInfo1.setId(bsBookInfo.getId());
        bsBookInfo1.setbNumber(integer);
        Integer line = bookService.updateBookInfoByKey(bsBookInfo1);
        if(line>0){
            return ResultBean.returnOk();
        }
        return ResultBean.returnError();
    }


}
