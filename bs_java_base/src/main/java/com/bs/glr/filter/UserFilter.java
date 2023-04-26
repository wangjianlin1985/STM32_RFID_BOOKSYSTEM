package com.bs.glr.filter;


import com.alibaba.druid.util.StringUtils;

import com.bs.glr.bean.BsUser;
import com.bs.glr.service.UserService;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.redis.core.StringRedisTemplate;

import javax.servlet.*;
import javax.servlet.annotation.WebFilter;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletRequestWrapper;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.*;

/**
 * @author glq
 * @create 2021-01-10 13:47
 */
@WebFilter("/auth/*")
@Slf4j
public class UserFilter implements Filter {



    @Autowired
    private StringRedisTemplate redisTemplate;

    @Autowired
    private UserService userService;

    @Override
    public void init(FilterConfig var1) throws ServletException{

    }


    @Override
    public void destroy() {

    }
    @Override
    public void doFilter(ServletRequest servletRequest, ServletResponse servletResponse, FilterChain filterChain) throws IOException, ServletException {
        //.info("走了过滤器。。。。。。。");
        String token = servletRequest.getParameter("token");
        String username = userService.getUserNameByToken(token);

//        String username = redisTemplate.opsForValue().get("user_token:" + token);
        if (StringUtils.isEmpty(token) || StringUtils.isEmpty(username)) {
            //log.info("获取用户数据失败，登录失败，获取到的token：" + token + ",redis查出来的数据：" + username);
            PrintWriter writer = null;
            try {
                writer = servletResponse.getWriter();
                writer.print("-1");
            } catch (IOException e) {

            } finally {
                if (writer != null) {

                    writer.close();
                }
            }
            return;
        }
        List<BsUser> userList = userService.getUserByUname(username);


        HashMap m = new HashMap(servletRequest.getParameterMap());
        m.put("uname", username);
        if(userList!=null && userList.size()>0){
            m.put("uid",userList.get(0).getId());
        }
        HttpServletRequest req = (HttpServletRequest) servletRequest;
        ParameterRequestWrapper wrapRequest = new ParameterRequestWrapper(req, m);
        servletRequest = wrapRequest;
        filterChain.doFilter(servletRequest, servletResponse);
    }

    @SuppressWarnings("unchecked")
    public class ParameterRequestWrapper extends HttpServletRequestWrapper {
        private Map params;

        public ParameterRequestWrapper(HttpServletRequest request, Map newParams) {
            super(request);
            this.params = newParams;
        }

        @Override
        public Map getParameterMap() {
            return params;
        }

        @Override
        public Enumeration getParameterNames() {
            Vector l = new Vector(params.keySet());
            return l.elements();
        }

        @Override
        public String[] getParameterValues(String name) {
            Object v = params.get(name);
            if (v == null) {
                return null;
            } else if (v instanceof String[]) {
                return (String[]) v;
            } else if (v instanceof String) {
                return new String[]{(String) v};
            } else {
                return new String[]{v.toString()};
            }
        }
    }
}
