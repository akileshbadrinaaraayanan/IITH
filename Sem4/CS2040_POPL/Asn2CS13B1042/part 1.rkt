#lang racket

(define (hyphen n)
  (if(> n 0)
  (string-append "-" (hyphen (- n 1)))
  (string-append "" "")
  ))

(define (align var1 var2)
  (if (> var1 0)
      (string-append " " (align (- var1 1) var2))
      (number->string var2)
)  )


(define (func1 v1 v2)
  (cond
    ((= v1 0) v2)
    ((> v1 0) (func1 (- v1 1) v2) (string-append v2 " "))
    ))

(define (func v1 v2 v3 v4)
  (displayln (func1 v4 (align (- v3 (string-length (number->string (* v1 (remainder v2 10))))) (* v1 (remainder v2 10)))))
  (calculate v1 (quotient v2 10) (- v3 1) (+ v4 1))
)

(define (calculate v1 v2 v3 v4)
  (cond
    ((= v2 0) 0) 
    ((= (remainder v2 10) 0) (calculate v1 (quotient v2 10) (- v3 1) (+ v4 1)))
    ((> v2 0) (func v1 v2 v3 v4))
    ))

 

(define (mult n1 n2)
  (define n (+ (string-length (number->string n1)) (string-length (number->string n2))))
  (displayln (align (- n (string-length (number->string n1))) n1)) 
  (display "x")
  (displayln (align (- (- n 1) (string-length (number->string n2))) n2))
  (displayln (hyphen n))
    (calculate n1 n2 n 0)
    (displayln (hyphen n))
    (display (align (- n (string-length (number->string (* n1 n2)))) (* n1 n2)))
  )
