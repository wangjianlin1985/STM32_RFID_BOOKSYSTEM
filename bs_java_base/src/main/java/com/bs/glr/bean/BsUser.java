package com.bs.glr.bean;

public class BsUser {
    /**
     *
     * This field was generated by MyBatis Generator.
     * This field corresponds to the database column bs_user.id
     *
     * @mbg.generated Sun Feb 27 21:36:07 CST 2022
     */
    private Integer id;

    /**
     *
     * This field was generated by MyBatis Generator.
     * This field corresponds to the database column bs_user.uname
     *
     * @mbg.generated Sun Feb 27 21:36:07 CST 2022
     */
    private String uname;

    /**
     *
     * This field was generated by MyBatis Generator.
     * This field corresponds to the database column bs_user.pwd
     *
     * @mbg.generated Sun Feb 27 21:36:07 CST 2022
     */
    private String pwd;

    /**
     *
     * This field was generated by MyBatis Generator.
     * This field corresponds to the database column bs_user.identity
     *
     * @mbg.generated Sun Feb 27 21:36:07 CST 2022
     */
    private String identity;

    /**
     * This method was generated by MyBatis Generator.
     * This method returns the value of the database column bs_user.id
     *
     * @return the value of bs_user.id
     *
     * @mbg.generated Sun Feb 27 21:36:07 CST 2022
     */
    public Integer getId() {
        return id;
    }

    /**
     * This method was generated by MyBatis Generator.
     * This method sets the value of the database column bs_user.id
     *
     * @param id the value for bs_user.id
     *
     * @mbg.generated Sun Feb 27 21:36:07 CST 2022
     */
    public void setId(Integer id) {
        this.id = id;
    }

    /**
     * This method was generated by MyBatis Generator.
     * This method returns the value of the database column bs_user.uname
     *
     * @return the value of bs_user.uname
     *
     * @mbg.generated Sun Feb 27 21:36:07 CST 2022
     */
    public String getUname() {
        return uname;
    }

    /**
     * This method was generated by MyBatis Generator.
     * This method sets the value of the database column bs_user.uname
     *
     * @param uname the value for bs_user.uname
     *
     * @mbg.generated Sun Feb 27 21:36:07 CST 2022
     */
    public void setUname(String uname) {
        this.uname = uname == null ? null : uname.trim();
    }

    /**
     * This method was generated by MyBatis Generator.
     * This method returns the value of the database column bs_user.pwd
     *
     * @return the value of bs_user.pwd
     *
     * @mbg.generated Sun Feb 27 21:36:07 CST 2022
     */
    public String getPwd() {
        return pwd;
    }

    /**
     * This method was generated by MyBatis Generator.
     * This method sets the value of the database column bs_user.pwd
     *
     * @param pwd the value for bs_user.pwd
     *
     * @mbg.generated Sun Feb 27 21:36:07 CST 2022
     */
    public void setPwd(String pwd) {
        this.pwd = pwd == null ? null : pwd.trim();
    }

    /**
     * This method was generated by MyBatis Generator.
     * This method returns the value of the database column bs_user.identity
     *
     * @return the value of bs_user.identity
     *
     * @mbg.generated Sun Feb 27 21:36:07 CST 2022
     */
    public String getIdentity() {
        return identity;
    }

    /**
     * This method was generated by MyBatis Generator.
     * This method sets the value of the database column bs_user.identity
     *
     * @param identity the value for bs_user.identity
     *
     * @mbg.generated Sun Feb 27 21:36:07 CST 2022
     */
    public void setIdentity(String identity) {
        this.identity = identity == null ? null : identity.trim();
    }
}