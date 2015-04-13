CREATE TABLE IF NOT EXISTS mk_account (
  ac_id serial,
  ac_name varchar[32],
  ac_email varchar[128],
  ac_firstname varchar[32],
  ac_lastname varchar[32],
  ac_salt varchar[8],
  ac_password varchar[256],
  ac_joindate date,
  CONSTRAINT pk_ac_id PRIMARY KEY(ac_id)
);

CREATE TABLE IF NOT EXISTS mk_log_history (
  log_id serial,
  log_ac_id serial,
  CONSTRAINT pk_log_id PRIMARY KEY(log_id),
  CONSTRAINT fk_log_ac_id FOREIGN KEY(log_ac_id) REFERENCES mk_account(ac_id)
);

CREATE TABLE IF NOT EXISTS mk_character (
  ch_id serial,
  ch_ac_id serial,
  ch_class varchar[32],
  ch_createdate date,
  CONSTRAINT pk_ch_id PRIMARY KEY(ch_id),
  CONSTRAINT fk_ch_ac_id FOREIGN KEY(ch_ac_id) REFERENCES mk_account(ac_id)
);

CREATE TABLE IF NOT EXISTS mk_log_history_item (
  lhi_id serial,
  lhi_log_id serial,
  lhi_date time,
  CONSTRAINT pk_lhi_id PRIMARY KEY(lhi_id),
  CONSTRAINT fk_lhi_log_id FOREIGN KEY(lhi_log_id) REFERENCES mk_log_history(log_id)
);

CREATE TABLE IF NOT EXISTS mk_status (
  st_id serial,
  st_ch_id serial,
  st_hp int,
  st_mp int,
  CONSTRAINT pk_st_id PRIMARY KEY(st_id),
  CONSTRAINT fk_st_ch_id FOREIGN KEY(st_ch_id) REFERENCES mk_character(ch_id)
);

CREATE TABLE IF NOT EXISTS mk_points (
  pt_id serial,
  pt_st_id serial,
  pt_avail smallint,
  pt_force smallint,
  pt_defense smallint,
  pt_agility smallint,
  CONSTRAINT pk_pt_id PRIMARY KEY(pt_id),
  CONSTRAINT fk_st_id FOREIGN KEY(pt_st_id) REFERENCES mk_status(st_id)
);

CREATE TABLE IF NOT EXISTS mk_build (
  bd_id serial,
  bd_ac_id serial,
  bd_position int,
  bd_title varchar[32],
  bd_password varchar[25],
  bd_kind varchar[32],
  CONSTRAINT pk_bd_id PRIMARY KEY(bd_id),
  CONSTRAINT fk_bd_ac_id FOREIGN KEY(bd_ac_id) REFERENCES mk_account(ac_id)
);

CREATE TABLE IF NOT EXISTS mk_chest (
  cht_id serial,
  cht_bd_id serial,
  cht_name varchar[32],
  CONSTRAINT pk_cht_id PRIMARY KEY(cht_id),
  CONSTRAINT fk_cht_bd_id FOREIGN KEY(cht_bd_id) REFERENCES mk_build(bd_id)
);

CREATE TABLE IF NOT EXISTS mk_inventory (
  inv_id serial,
  inv_ch_id serial,
  CONSTRAINT pk_inv_id PRIMARY KEY(inv_id),
  CONSTRAINT fk_inv_ch_id FOREIGN KEY(inv_ch_id) REFERENCES mk_character(ch_id)
);

CREATE TABLE IF NOT EXISTS mk_artefact (
  atf_id serial,
  atf_ch_id serial,
  atf_name varchar[32],
  CONSTRAINT pk_atf_id PRIMARY KEY(atf_id),
  CONSTRAINT fk_atf_ch_id FOREIGN KEY(atf_ch_id) REFERENCES mk_character(ch_id)
);

CREATE TABLE IF NOT EXISTS mk_quest (
  qs_id serial,
  qs_ch_id serial,
  qs_name varchar[32],
  CONSTRAINT pk_qs_id PRIMARY KEY(qs_id),
  CONSTRAINT fk_qs_ch_id FOREIGN KEY(qs_ch_id) REFERENCES mk_character(ch_id)
);

CREATE TABLE IF NOT EXISTS mk_inventory_object (
  invo_id serial,
  invo_inv_id serial,
  invo_name varchar[32],
  invo_count smallint,
  CONSTRAINT pk_invo_id PRIMARY KEY(invo_id),
  CONSTRAINT fk_invo_inv_id FOREIGN KEY(invo_inv_id) REFERENCES mk_inventory(inv_id)
);

CREATE TABLE IF NOT EXISTS mk_quest_property (
  qp_id serial,
  qp_qs_id serial,
  qp_name varchar[32],
  qp_value text,
  CONSTRAINT pk_qp_id PRIMARY KEY(qp_id),
  CONSTRAINT fk_qp_qs_id FOREIGN KEY(qp_qs_id) REFERENCES mk_quest(qs_id)
);