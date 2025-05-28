-- Matrix Online Emulator Database Schema
-- Based on reverse engineering of the MxoEmu codebase

-- Drop tables if they exist
DROP TABLE IF EXISTS `dialogue_history`;
DROP TABLE IF EXISTS `dialogue_actions`;
DROP TABLE IF EXISTS `dialogue_options`;
DROP TABLE IF EXISTS `dialogue_entries`;
DROP TABLE IF EXISTS `mission_prerequisites`;
DROP TABLE IF EXISTS `mission_objectives`;
DROP TABLE IF EXISTS `mission_instances`;
DROP TABLE IF EXISTS `mission_completed`;
DROP TABLE IF EXISTS `mission_definitions`;
DROP TABLE IF EXISTS `districts`;
DROP TABLE IF EXISTS `character_appearance`;
DROP TABLE IF EXISTS `character_inventory`;
DROP TABLE IF EXISTS `character_skills`;
DROP TABLE IF EXISTS `characters`;
DROP TABLE IF EXISTS `worlds`;
DROP TABLE IF EXISTS `accounts`;

-- Accounts table
CREATE TABLE `accounts` (
  `account_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(50) NOT NULL,
  `password_hash` varchar(40) NOT NULL COMMENT 'SHA-1 hash of salt+password',
  `password_salt` varchar(32) NOT NULL COMMENT 'Random salt for password hashing',
  `email` varchar(100) DEFAULT NULL,
  `last_login` datetime DEFAULT NULL,
  `last_ip` varchar(15) DEFAULT NULL,
  `creation_date` datetime DEFAULT CURRENT_TIMESTAMP,
  `is_admin` tinyint(1) NOT NULL DEFAULT '0',
  `is_banned` tinyint(1) NOT NULL DEFAULT '0',
  `ban_reason` text,
  `ban_expiry` datetime DEFAULT NULL,
  PRIMARY KEY (`account_id`),
  UNIQUE KEY `username` (`username`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='User account information';

-- Worlds table
CREATE TABLE `worlds` (
  `world_id` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `world_name` varchar(50) NOT NULL,
  `status` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '0=offline, 1=online, 2=maintenance',
  `address` varchar(100) DEFAULT NULL COMMENT 'Server address',
  `port` smallint(5) unsigned DEFAULT NULL COMMENT 'Server port',
  `population` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT 'Current player count',
  `max_population` smallint(5) unsigned NOT NULL DEFAULT '500' COMMENT 'Maximum players allowed',
  PRIMARY KEY (`world_id`),
  UNIQUE KEY `world_name` (`world_name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Game world definitions';

-- Characters table
CREATE TABLE `characters` (
  `character_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `account_id` int(10) unsigned NOT NULL,
  `world_id` smallint(5) unsigned NOT NULL,
  `character_handle` varchar(32) NOT NULL COMMENT 'Unique character name',
  `first_name` varchar(32) NOT NULL,
  `last_name` varchar(32) NOT NULL,
  `background` text COMMENT 'Character backstory',
  `experience` bigint(20) unsigned NOT NULL DEFAULT '0',
  `information` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'Currency',
  `health_current` smallint(5) unsigned NOT NULL DEFAULT '100',
  `health_max` smallint(5) unsigned NOT NULL DEFAULT '100',
  `innerstr_current` smallint(5) unsigned NOT NULL DEFAULT '100',
  `innerstr_max` smallint(5) unsigned NOT NULL DEFAULT '100',
  `profession` int(10) unsigned NOT NULL DEFAULT '0',
  `level` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `alignment` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '0=neutral, 1=zion, 2=machine, 3=merovingian',
  `pos_x` double NOT NULL DEFAULT '0',
  `pos_y` double NOT NULL DEFAULT '0',
  `pos_z` double NOT NULL DEFAULT '0',
  `rotation` double NOT NULL DEFAULT '0',
  `district` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `is_online` tinyint(1) NOT NULL DEFAULT '0',
  `last_online` datetime DEFAULT NULL,
  `total_time_played` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'In seconds',
  `creation_date` datetime DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`character_id`),
  UNIQUE KEY `character_handle` (`character_handle`),
  KEY `account_id` (`account_id`),
  KEY `world_id` (`world_id`),
  CONSTRAINT `characters_ibfk_1` FOREIGN KEY (`account_id`) REFERENCES `accounts` (`account_id`) ON DELETE CASCADE,
  CONSTRAINT `characters_ibfk_2` FOREIGN KEY (`world_id`) REFERENCES `worlds` (`world_id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Character data';

-- Character appearance data
CREATE TABLE `character_appearance` (
  `character_id` bigint(20) unsigned NOT NULL,
  `rsi_data` blob NOT NULL COMMENT 'Serialized appearance data',
  `current_animation` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `current_mood` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`character_id`),
  CONSTRAINT `character_appearance_ibfk_1` FOREIGN KEY (`character_id`) REFERENCES `characters` (`character_id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Character appearance data';

-- Character inventory
CREATE TABLE `character_inventory` (
  `inventory_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `character_id` bigint(20) unsigned NOT NULL,
  `item_id` int(10) unsigned NOT NULL,
  `item_count` int(10) unsigned NOT NULL DEFAULT '1',
  `item_data` blob COMMENT 'Serialized item data',
  `slot_id` int(10) unsigned NOT NULL DEFAULT '0',
  `is_equipped` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`inventory_id`),
  KEY `character_id` (`character_id`),
  CONSTRAINT `character_inventory_ibfk_1` FOREIGN KEY (`character_id`) REFERENCES `characters` (`character_id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Character inventory data';

-- Character skills
CREATE TABLE `character_skills` (
  `skill_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `character_id` bigint(20) unsigned NOT NULL,
  `skill_type` int(10) unsigned NOT NULL,
  `skill_level` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `skill_experience` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`skill_id`),
  UNIQUE KEY `character_id_skill_type` (`character_id`,`skill_type`),
  CONSTRAINT `character_skills_ibfk_1` FOREIGN KEY (`character_id`) REFERENCES `characters` (`character_id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Character skills';

-- Districts table
CREATE TABLE `districts` (
  `district_id` tinyint(3) unsigned NOT NULL,
  `name` varchar(50) NOT NULL,
  `description` text,
  `navmesh_file` varchar(255) DEFAULT NULL COMMENT 'Path to navmesh file',
  PRIMARY KEY (`district_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Game world districts';

-- District adjacency
CREATE TABLE `district_adjacency` (
  `district_id` tinyint(3) unsigned NOT NULL,
  `adjacent_district_id` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`district_id`,`adjacent_district_id`),
  KEY `adjacent_district_id` (`adjacent_district_id`),
  CONSTRAINT `district_adjacency_ibfk_1` FOREIGN KEY (`district_id`) REFERENCES `districts` (`district_id`),
  CONSTRAINT `district_adjacency_ibfk_2` FOREIGN KEY (`adjacent_district_id`) REFERENCES `districts` (`district_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='District adjacency data';

-- Hardline positions
CREATE TABLE `hardline_positions` (
  `hardline_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `district_id` tinyint(3) unsigned NOT NULL,
  `pos_x` double NOT NULL,
  `pos_y` double NOT NULL,
  `pos_z` double NOT NULL,
  `name` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`hardline_id`),
  KEY `district_id` (`district_id`),
  CONSTRAINT `hardline_positions_ibfk_1` FOREIGN KEY (`district_id`) REFERENCES `districts` (`district_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Hardline teleport positions';

-- Spawn positions
CREATE TABLE `spawn_positions` (
  `spawn_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `district_id` tinyint(3) unsigned NOT NULL,
  `pos_x` double NOT NULL,
  `pos_y` double NOT NULL,
  `pos_z` double NOT NULL,
  `rotation` double NOT NULL DEFAULT '0',
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '0=normal, 1=newbie, 2=faction',
  `faction` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '0=any, 1=zion, 2=machine, 3=merovingian',
  PRIMARY KEY (`spawn_id`),
  KEY `district_id` (`district_id`),
  CONSTRAINT `spawn_positions_ibfk_1` FOREIGN KEY (`district_id`) REFERENCES `districts` (`district_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player spawn positions';

-- Mission definitions
CREATE TABLE `mission_definitions` (
  `mission_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  `description` text,
  `min_level` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `max_level` tinyint(3) unsigned NOT NULL DEFAULT '50',
  `faction` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '0=any, 1=zion, 2=machine, 3=merovingian',
  `giver_npc_id` int(10) unsigned NOT NULL,
  `turnin_npc_id` int(10) unsigned NOT NULL,
  `repeatable` tinyint(1) NOT NULL DEFAULT '0',
  `cooldown_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Seconds before mission can be repeated',
  `start_dialogue` text,
  `completion_dialogue` text,
  `failure_dialogue` text,
  PRIMARY KEY (`mission_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Mission definitions';

-- Mission objectives
CREATE TABLE `mission_objectives` (
  `objective_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `mission_id` int(10) unsigned NOT NULL,
  `description` text NOT NULL,
  `target_value` int(10) unsigned NOT NULL DEFAULT '1',
  `is_optional` tinyint(1) NOT NULL DEFAULT '0',
  `completion_text` text,
  `reward_experience` int(10) unsigned NOT NULL DEFAULT '0',
  `reward_information` int(10) unsigned NOT NULL DEFAULT '0',
  `sequence` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Order in which objectives appear',
  PRIMARY KEY (`objective_id`),
  KEY `mission_id` (`mission_id`),
  CONSTRAINT `mission_objectives_ibfk_1` FOREIGN KEY (`mission_id`) REFERENCES `mission_definitions` (`mission_id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Mission objectives';

-- Mission prerequisites
CREATE TABLE `mission_prerequisites` (
  `prerequisite_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `mission_id` int(10) unsigned NOT NULL,
  `type` tinyint(3) unsigned NOT NULL COMMENT '0=level, 1=faction, 2=profession, 3=completed_mission, 4=item, 5=skill, 6=district',
  `value` int(10) unsigned NOT NULL,
  PRIMARY KEY (`prerequisite_id`),
  KEY `mission_id` (`mission_id`),
  CONSTRAINT `mission_prerequisites_ibfk_1` FOREIGN KEY (`mission_id`) REFERENCES `mission_definitions` (`mission_id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Mission prerequisites';

-- Mission reward items
CREATE TABLE `mission_reward_items` (
  `reward_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `mission_id` int(10) unsigned NOT NULL,
  `objective_id` int(10) unsigned DEFAULT NULL COMMENT 'NULL means mission completion reward',
  `item_id` int(10) unsigned NOT NULL,
  `item_count` int(10) unsigned NOT NULL DEFAULT '1',
  PRIMARY KEY (`reward_id`),
  KEY `mission_id` (`mission_id`),
  KEY `objective_id` (`objective_id`),
  CONSTRAINT `mission_reward_items_ibfk_1` FOREIGN KEY (`mission_id`) REFERENCES `mission_definitions` (`mission_id`) ON DELETE CASCADE,
  CONSTRAINT `mission_reward_items_ibfk_2` FOREIGN KEY (`objective_id`) REFERENCES `mission_objectives` (`objective_id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Mission reward items';

-- Active mission instances
CREATE TABLE `mission_instances` (
  `instance_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `character_id` bigint(20) unsigned NOT NULL,
  `mission_id` int(10) unsigned NOT NULL,
  `start_time` int(10) unsigned NOT NULL COMMENT 'Unix timestamp',
  `is_completed` tinyint(1) NOT NULL DEFAULT '0',
  `is_failed` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`instance_id`),
  UNIQUE KEY `character_id_mission_id` (`character_id`,`mission_id`),
  KEY `mission_id` (`mission_id`),
  CONSTRAINT `mission_instances_ibfk_1` FOREIGN KEY (`character_id`) REFERENCES `characters` (`character_id`) ON DELETE CASCADE,
  CONSTRAINT `mission_instances_ibfk_2` FOREIGN KEY (`mission_id`) REFERENCES `mission_definitions` (`mission_id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Active mission instances';

-- Mission objective progress
CREATE TABLE `mission_objective_progress` (
  `progress_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `instance_id` bigint(20) unsigned NOT NULL,
  `objective_id` int(10) unsigned NOT NULL,
  `progress` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`progress_id`),
  UNIQUE KEY `instance_id_objective_id` (`instance_id`,`objective_id`),
  KEY `objective_id` (`objective_id`),
  CONSTRAINT `mission_objective_progress_ibfk_1` FOREIGN KEY (`instance_id`) REFERENCES `mission_instances` (`instance_id`) ON DELETE CASCADE,
  CONSTRAINT `mission_objective_progress_ibfk_2` FOREIGN KEY (`objective_id`) REFERENCES `mission_objectives` (`objective_id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Mission objective progress';

-- Completed missions
CREATE TABLE `mission_completed` (
  `completion_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `character_id` bigint(20) unsigned NOT NULL,
  `mission_id` int(10) unsigned NOT NULL,
  `completion_time` int(10) unsigned NOT NULL COMMENT 'Unix timestamp',
  `completion_count` int(10) unsigned NOT NULL DEFAULT '1' COMMENT 'Number of times completed',
  PRIMARY KEY (`completion_id`),
  UNIQUE KEY `character_id_mission_id` (`character_id`,`mission_id`),
  KEY `mission_id` (`mission_id`),
  CONSTRAINT `mission_completed_ibfk_1` FOREIGN KEY (`character_id`) REFERENCES `characters` (`character_id`) ON DELETE CASCADE,
  CONSTRAINT `mission_completed_ibfk_2` FOREIGN KEY (`mission_id`) REFERENCES `mission_definitions` (`mission_id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Completed missions';

-- Dialogue entries
CREATE TABLE `dialogue_entries` (
  `dialogue_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `npc_id` int(10) unsigned NOT NULL,
  `text` text NOT NULL,
  `npc_emotion` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `npc_animation` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `is_initial` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Is this the initial dialogue for the NPC',
  PRIMARY KEY (`dialogue_id`),
  KEY `npc_id` (`npc_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='NPC dialogue entries';

-- Dialogue options
CREATE TABLE `dialogue_options` (
  `option_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `dialogue_id` int(10) unsigned NOT NULL,
  `text` text NOT NULL,
  `next_dialogue_id` int(10) unsigned NOT NULL,
  `end_conversation` tinyint(1) NOT NULL DEFAULT '0',
  `required_mission_id` int(10) unsigned NOT NULL DEFAULT '0',
  `required_mission_state` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '0=not started, 1=in progress, 2=completed',
  `required_level` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `required_faction` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `required_skill_id` int(10) unsigned NOT NULL DEFAULT '0',
  `required_skill_level` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`option_id`),
  KEY `dialogue_id` (`dialogue_id`),
  KEY `next_dialogue_id` (`next_dialogue_id`),
  CONSTRAINT `dialogue_options_ibfk_1` FOREIGN KEY (`dialogue_id`) REFERENCES `dialogue_entries` (`dialogue_id`) ON DELETE CASCADE,
  CONSTRAINT `dialogue_options_ibfk_2` FOREIGN KEY (`next_dialogue_id`) REFERENCES `dialogue_entries` (`dialogue_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Dialogue response options';

-- Dialogue actions
CREATE TABLE `dialogue_actions` (
  `action_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `dialogue_id` int(10) unsigned NOT NULL,
  `type` tinyint(3) unsigned NOT NULL COMMENT '0=none, 1=start_mission, 2=complete_mission, 3=give_item, 4=take_item, 5=teleport, 6=open_shop, 7=train_skill, 8=set_faction',
  `value` int(10) unsigned NOT NULL DEFAULT '0',
  `secondary_value` int(10) unsigned NOT NULL DEFAULT '0',
  `action_text` text,
  PRIMARY KEY (`action_id`),
  KEY `dialogue_id` (`dialogue_id`),
  CONSTRAINT `dialogue_actions_ibfk_1` FOREIGN KEY (`dialogue_id`) REFERENCES `dialogue_entries` (`dialogue_id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Actions triggered by dialogue';

-- Dialogue history
CREATE TABLE `dialogue_history` (
  `history_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `character_id` bigint(20) unsigned NOT NULL,
  `npc_id` int(10) unsigned NOT NULL,
  `dialogue_id` int(10) unsigned NOT NULL,
  `timestamp` int(10) unsigned NOT NULL COMMENT 'Unix timestamp',
  PRIMARY KEY (`history_id`),
  KEY `character_id_npc_id` (`character_id`,`npc_id`),
  KEY `dialogue_id` (`dialogue_id`),
  CONSTRAINT `dialogue_history_ibfk_1` FOREIGN KEY (`character_id`) REFERENCES `characters` (`character_id`) ON DELETE CASCADE,
  CONSTRAINT `dialogue_history_ibfk_2` FOREIGN KEY (`dialogue_id`) REFERENCES `dialogue_entries` (`dialogue_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player dialogue history';