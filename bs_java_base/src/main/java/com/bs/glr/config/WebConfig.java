package com.bs.glr.config;

import org.springframework.context.annotation.Configuration;
import org.springframework.core.Ordered;
import org.springframework.web.servlet.config.annotation.CorsRegistry;
import org.springframework.web.servlet.config.annotation.InterceptorRegistry;
import org.springframework.web.servlet.config.annotation.ViewControllerRegistry;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurer;

@Configuration //加配置注解可以扫描到
public class WebConfig implements WebMvcConfigurer{

    //跨域请求配置
    @Override
    public void addCorsMappings(CorsRegistry registry) {
        WebMvcConfigurer.super.addCorsMappings(registry);
        registry.addMapping("/**")// 对接口配置跨域设置
                .allowedHeaders("*")// 允许任何头
                .allowedMethods("POST","GET")// 允许方法（post、get等）
               // .allowedOrigins("*")// 允许任何域名使用
                .allowedOriginPatterns("*")
                .allowCredentials(true);
    }

}