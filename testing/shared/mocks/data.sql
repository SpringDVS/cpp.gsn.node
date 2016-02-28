BEGIN TRANSACTION;
CREATE TABLE "geosub_meta" (
	`id`	INTEGER PRIMARY KEY AUTOINCREMENT,
	`settlement`	TEXT,
	`postcode`	TEXT,
	`county`	TEXT,
	`geosub`	TEXT
);
INSERT INTO `geosub_meta` (id,settlement,postcode,county,geosub) VALUES (3,'St-Leonards-On-Sea','tn37','East Sussex','esusx');
INSERT INTO `geosub_meta` (id,settlement,postcode,county,geosub) VALUES (4,'Hastings','tn34','East Sussex','esusx');
INSERT INTO `geosub_meta` (id,settlement,postcode,county,geosub) VALUES (5,'Romney Marsh','tn29','Kent','kent');
COMMIT;
