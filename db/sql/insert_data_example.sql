INSERT OR REPLACE INTO "serial" VALUES
('ttyUSB0', 9600, "8n1");

INSERT OR REPLACE INTO channel(sensor_id,heater_id,cooler_id,ambient_temperature,matter_mass,matter_ksh,loss_factor,loss_power,temperature_pipe_length,cycle_duration_sec,cycle_duration_nsec) VALUES 
(1,11,12, 20.0,500,1200,1.0,2.0,100, 0,500000000),
(2,21,22, 20.0,500,1200,1.0,2.0,100, 0,500000000),
(3,31,32, 20.0,500,1200,1.0,2.0,100, 0,500000000),
(4,41,42, 20.0,500,1200,1.0,2.0,100, 0,500000000);




