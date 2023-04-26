package com.bs.glr.bean;



import lombok.Data;

import java.util.HashMap;
import java.util.Map;

/**
 * @author glr
 * @create 2020-07-18 15:44
 */
@Data
public class ResultBean {

    private boolean success;

    private Integer code;


    private String message;


    private Map<String, Object> data = new HashMap<String, Object>();

    private ResultBean(){}

    public static ResultBean returnOk(){
        ResultBean r = new ResultBean();
        r.setSuccess(true);
        r.setCode(20000);
        r.setMessage("成功");
        return r;
    }

    public static ResultBean returnOk(String message){
        ResultBean r = new ResultBean();
        r.setSuccess(true);
        r.setCode(20000);
        r.setMessage(message);
        return r;
    }

    public static ResultBean returnError(){
        ResultBean r = new ResultBean();
        r.setSuccess(false);
        r.setCode(99999);
        r.setMessage("失败");
        return r;
    }

    public static ResultBean returnError(String message){
        ResultBean r = new ResultBean();
        r.setSuccess(false);
        r.setCode(99999);
        r.setMessage(message);
        return r;
    }

    public ResultBean pushMessage(String message){
        this.setMessage(message);
        return this;
    }

    public ResultBean code(Integer code){
        this.setCode(code);
        return this;
    }

    public ResultBean pushData(String key, Object value){
        this.data.put(key, value);
        return this;
    }

    public ResultBean pushData(Map<String, Object> map){
        this.setData(map);
        return this;
    }

    public ResultBean(boolean success, Integer code, String message) {
        this.success = success;
        this.code = code;
        this.message = message;

    }
}
