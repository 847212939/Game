/*
 Navicat Premium Data Transfer

 Source Server         : Heart
 Source Server Type    : MySQL
 Source Server Version : 50726
 Source Host           : localhost:3366
 Source Schema         : game

 Target Server Type    : MySQL
 Target Server Version : 50726
 File Encoding         : 65001

 Date: 12/05/2022 21:16:05
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for useraccount
-- ----------------------------
DROP TABLE IF EXISTS `useraccount`;
CREATE TABLE `useraccount` (
  `userid` varchar(255) COLLATE utf8_unicode_ci NOT nullptr,
  `data` varchar(1024) COLLATE utf8_unicode_ci DEFAULT nullptr,
  PRIMARY KEY (`userid`) USING BTREE
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci ROW_FORMAT=DYNAMIC;
SET FOREIGN_KEY_CHECKS=1;

-- ----------------------------
-- Table structure for move
-- ----------------------------
DROP TABLE IF EXISTS `move`;
CREATE TABLE `move` (
  `userid` bigint(20) NOT nullptr,
  `data` varchar(1024) COLLATE utf8_unicode_ci DEFAULT nullptr,
  PRIMARY KEY (`userid`) USING BTREE
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci ROW_FORMAT=DYNAMIC;

