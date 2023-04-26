package com.bs.glr.bean;

import org.apache.http.HttpResponse;
import org.apache.http.util.EntityUtils;

import java.util.HashMap;
import java.util.Map;

/**
 * @author glr
 * @create 2021-02-01 16:01
 * @desc ${var}
 */
public class MessageUtil {

    public static Boolean sendMessage(String phone,String code) {
        String host = "https://zwp.market.alicloudapi.com";
        String path = "/sms/sendv2";
        String method = "GET";
        String appcode = "6d8a9ac9655a4623a2b21b8d2df2d1fc";
        Map<String, String> headers = new HashMap<String, String>();
        //最后在header中的格式(中间是英文空格)为Authorization:APPCODE 83359fd73fe94948385f570e3c139105
        headers.put("Authorization", "APPCODE " + appcode);
        Map<String, String> querys = new HashMap<String, String>();
        querys.put("content", "【儿童教务】您正在登录验证,验证码为"+code+" ,60s内有效,请尽快验证。");
        querys.put("mobile", phone);


        try {
            /**
             * 重要提示如下:
             * HttpUtils请从
             * https://github.com/aliyun/api-gateway-demo-sign-java/blob/master/src/main/java/com/aliyun/api/gateway/demo/util/HttpUtils.java
             * 下载
             *
             * 相应的依赖请参照
             * https://github.com/aliyun/api-gateway-demo-sign-java/blob/master/pom.xml
             */
            HttpResponse response = HttpUtils.doGet(host, path, method, headers, querys);
            System.out.println(response.toString());
//            获取response的body
            String resp = EntityUtils.toString(response.getEntity());
            System.out.println(resp);
            if(resp.indexOf("error_code\":0") != -1){
                System.out.println("发送成功");
                return true;
            }else{
                System.out.println("发送失败");
                return false;
            }
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }
}
