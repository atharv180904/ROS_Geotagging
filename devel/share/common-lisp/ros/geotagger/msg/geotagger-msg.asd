
(cl:in-package :asdf)

(defsystem "geotagger-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "MavrosData" :depends-on ("_package_MavrosData"))
    (:file "_package_MavrosData" :depends-on ("_package"))
  ))