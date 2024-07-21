;; Auto-generated. Do not edit!


(when (boundp 'geotagger::MavrosData)
  (if (not (find-package "GEOTAGGER"))
    (make-package "GEOTAGGER"))
  (shadow 'MavrosData (find-package "GEOTAGGER")))
(unless (find-package "GEOTAGGER::MAVROSDATA")
  (make-package "GEOTAGGER::MAVROSDATA"))

(in-package "ROS")
;;//! \htmlinclude MavrosData.msg.html


(defclass geotagger::MavrosData
  :super ros::object
  :slots (_latitude _longitude _altitude _compass_heading _yaw _timestamp ))

(defmethod geotagger::MavrosData
  (:init
   (&key
    ((:latitude __latitude) 0.0)
    ((:longitude __longitude) 0.0)
    ((:altitude __altitude) 0.0)
    ((:compass_heading __compass_heading) 0.0)
    ((:yaw __yaw) 0.0)
    ((:timestamp __timestamp) (instance ros::time :init))
    )
   (send-super :init)
   (setq _latitude (float __latitude))
   (setq _longitude (float __longitude))
   (setq _altitude (float __altitude))
   (setq _compass_heading (float __compass_heading))
   (setq _yaw (float __yaw))
   (setq _timestamp __timestamp)
   self)
  (:latitude
   (&optional __latitude)
   (if __latitude (setq _latitude __latitude)) _latitude)
  (:longitude
   (&optional __longitude)
   (if __longitude (setq _longitude __longitude)) _longitude)
  (:altitude
   (&optional __altitude)
   (if __altitude (setq _altitude __altitude)) _altitude)
  (:compass_heading
   (&optional __compass_heading)
   (if __compass_heading (setq _compass_heading __compass_heading)) _compass_heading)
  (:yaw
   (&optional __yaw)
   (if __yaw (setq _yaw __yaw)) _yaw)
  (:timestamp
   (&optional __timestamp)
   (if __timestamp (setq _timestamp __timestamp)) _timestamp)
  (:serialization-length
   ()
   (+
    ;; float64 _latitude
    8
    ;; float64 _longitude
    8
    ;; float64 _altitude
    8
    ;; float64 _compass_heading
    8
    ;; float64 _yaw
    8
    ;; time _timestamp
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64 _latitude
       (sys::poke _latitude (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _longitude
       (sys::poke _longitude (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _altitude
       (sys::poke _altitude (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _compass_heading
       (sys::poke _compass_heading (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _yaw
       (sys::poke _yaw (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; time _timestamp
       (write-long (send _timestamp :sec) s) (write-long (send _timestamp :nsec) s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64 _latitude
     (setq _latitude (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _longitude
     (setq _longitude (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _altitude
     (setq _altitude (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _compass_heading
     (setq _compass_heading (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _yaw
     (setq _yaw (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; time _timestamp
     (send _timestamp :sec (sys::peek buf ptr- :integer)) (incf ptr- 4)  (send _timestamp :nsec (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get geotagger::MavrosData :md5sum-) "504b4cf00b55003d0350cb2c79a82ffc")
(setf (get geotagger::MavrosData :datatype-) "geotagger/MavrosData")
(setf (get geotagger::MavrosData :definition-)
      "# geotagger/MavrosData.msg

#include \"std_msgs/Header.msg\"


float64 latitude
float64 longitude
float64 altitude
float64 compass_heading
float64 yaw
time timestamp

")



(provide :geotagger/MavrosData "504b4cf00b55003d0350cb2c79a82ffc")


