package com.bs.glr;

import org.mybatis.spring.annotation.MapperScan;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.web.servlet.ServletComponentScan;

@MapperScan("com.bs.glr.mapper")
@SpringBootApplication
@ServletComponentScan
public class BsGlrApplication {

	public static void main(String[] args) {
		SpringApplication.run(BsGlrApplication.class, args);
	}

}
