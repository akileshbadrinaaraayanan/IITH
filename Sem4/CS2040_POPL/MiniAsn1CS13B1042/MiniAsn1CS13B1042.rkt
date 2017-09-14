;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-advanced-reader.ss" "lang")((modname MiniAsn1CS13B1042) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #t #t none #f ())))

(define (lstarts list1 list2)
    (cond
      [(null? list1) #t]
      [(null? list2) #f]
      [(eq? (car list1) (car list2)) (lstarts (cdr list1) (cdr list2))]
      [else #f]))

(define (lcontains list1 list2)
    (cond
    ((null? list2) (null? list1))
    [(null? list1) #f]
    [(lstarts list1 list2) #t]
    [else (lcontains list1 (cdr list2))]))

(define (lcount list1 list2)
  (
    if(null? list2) 
       0
       (if(lstarts list1 list2) 
          (+ 1 (lcount list1 (cdr list2)))  
          (lcount list1 (cdr list2))))
    
    )

(define (lremove list1 list2)
   (cond
      ((null? list1) list2)
      ((eq? (car list1) (car list2)) (lremove (cdr list1) (cdr list2)))
      (else #f)))

(define (ldelete list1 list2)
  (cond
    ((null? list2) null)
    ((lstarts list1 list2) (lremove list1 list2))
    (else (cons (car list2)(ldelete list1 (cdr list2))))))

(define (linsert list1 list2 l3)
  (cond
    ((null? l3) null)
    ((lstarts list2 l3)(append list2 (append list1 (lremove list2 l3))))
    (else (cons (car l3)(linsert list1 list2 (cdr l3))))))

(define (lreplace list1 list2 l3)
  (cond
    ((null? l3) null)
    ((lstarts list1 l3)(append list2 (lremove list1 l3)))
    (else (cons (car l3)(lreplace list1 list2 (cdr l3))))))

    

    
