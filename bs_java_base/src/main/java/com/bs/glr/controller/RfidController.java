package com.bs.glr.controller;

import com.bs.glr.bean.BsRfid;
import com.bs.glr.bean.ResultBean;
import com.bs.glr.service.RfidService;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import javax.annotation.Resource;

/**
 * @作者 GLQ
 * @时间 2022-02-28 17:56
 * @描述 ${var}
 */

@RestController
public class RfidController {

    @Resource
    RfidService rfidService;

    //获取数据前调用，将状态更新为0  -- 功能废弃
    @RequestMapping("/auth/book/rfid/startGetRfid")
    public ResultBean startGetRfid (){
        Integer line = rfidService.updateStatus("1");
        if(line>0){
            return ResultBean.returnOk();
        }
        return ResultBean.returnError();
    }
    //获取数据
    @RequestMapping("/auth/book/rfid/getRfid")
    public ResultBean getRfid (){
        BsRfid bsRfid = rfidService.getRfidId();
        if("1".equals(bsRfid.getRfidStatus())){
            return ResultBean.returnError("暂未获取到卡号，请重试");
        }
        rfidService.setRfidStatusToOne();

        return ResultBean.returnOk().pushData("data",bsRfid.getRfidId());
    }


    //设置数据
    @RequestMapping("/user/puttRfid")
    public ResultBean getRfid (String rfidId){
        System.out.println("接收到硬件传输的数据："+rfidId);
        Integer line = rfidService.insertRfid(rfidId);
        if(line>0){
            return ResultBean.returnOk();
        }
        return ResultBean.returnError();

    }

    //探测
    @RequestMapping("/user/test")
    public ResultBean test (){
        System.out.println("test.....");
        return ResultBean.returnOk();


    }

}
