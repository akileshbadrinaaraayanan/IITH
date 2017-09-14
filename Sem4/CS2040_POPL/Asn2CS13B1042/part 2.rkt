#lang racket

(define (present v1 v2)
  (cond
    ((null? v2) #f)
    ((= v1 (car v2)) #t)
    (else (present v1 (cdr v2)))))

(define (find-index v1 v2 v3)
  (cond
    ((null? v2) )
    ((= v1 (car v2)) (- v3 (length v2)))
    (else (find-index v1 (cdr v2) v3))))

(define (prt-spaces n)
   (cond
     ((> n 0) (display " ") (prt-spaces (- n 1)))
     ((<= n 0) (display ""))
  ))

(define (prt-hyphen n)
   (cond
     ((> n 0) (display "-") (prt-hyphen (- n 1)))
     ((<= n 0) (display ""))
  ))

(define (prt-symbol n)
   (cond
     ((> n 0) (display "_") (prt-symbol (- n 1)))
     ((<= n 0) (display ""))
  ))
    

(define (temp2 v1 v2)
  (append v1 (cons v2 null)))

(define (imp v1 v2 v3 v4)
  (cond
    ((= v1 0) (display ""))
    ((< v1 v2) (imp (* v1 10) v2 v3 v4))
    ((> v1 v2) 
     (cond 
      ((present (remainder v1 v2) v3)  
  (prt-spaces v4)
  (prt-spaces (find-index (remainder v1 v2) v3 (length v3)))
  (prt-symbol (- (length v3) (find-index (remainder v1 v2) v3 (length v3))))
  (\newline)    
  )                  
 (else (imp (remainder v1 v2) v2 (temp2 v3 (remainder v1 v2)) v4))))))

(define (temp v1 v2 v3)
  (cond
    ((= v1 0) (display ""))
    ((< v1 v2) (temp (* v1 10) v2 v3))
    ((> v1 v2) (display (quotient v1 v2)) (cond ((present (remainder v1 v2) v3) (display "")) (else (temp (remainder v1 v2) v2 (temp2 v3 (remainder v1 v2))))))))
  
(define (aft v1 v2 v3 v4)
  (cond
    ((< v1 v2)
     (cond
       ((present v1 v3)
        (prt-spaces v4)
        (display (* v1 10))        
        )
        
        (else
         (\newline)
         (prt-spaces v4)
         (displayln (* v1 10))
         (prt-spaces (+ v4 (- (string-length (number->string (* v1 10))) (string-length (number->string (* (quotient (* v1 10) v2) v2))))))
        (displayln (* (quotient (* v1 10) v2) v2))
        (prt-spaces v4)
        (prt-hyphen (string-length (number->string (* v1 10))))
        (\newline)
        (aft (remainder (* v1 10) v2) v2 (temp2 v3 v1) (+ v4 (- (string-length (number->string (* v1 10))) (string-length (number->string (remainder (* v1 10) v2)))))))))))       

(define (div num1 num2)
  (define l1 (cons (remainder num1 num2) null))
  (prt-spaces (+ 1 (string-length (number->string num2))))
  (imp (remainder num1 num2) num2 l1 (string-length (number->string (quotient num1 num2))))
  (prt-spaces (+ 1 (string-length (number->string num2))))
  (display (quotient num1 num2))
  (temp (remainder num1 num2) num2 l1)
  (\newline)
  (prt-spaces (+ 1 (string-length (number->string num2))))
  (prt-hyphen (- (string-length (number->string (quotient num1 num2))) 1))
  (display "^")
  (\newline)
  (prt-spaces (string-length (number->string num2)))
  (display ",")
  (\newline)
  (display num2)
  (display "|")
  (display num1)
  (define l2 '())
  (aft (remainder num1 num2) num2 l2 (+ (+ 1 (string-length (number->string num2))) (- (string-length (number->string num1)) (string-length (number->string (remainder num1 num2)))))))
  
  
  