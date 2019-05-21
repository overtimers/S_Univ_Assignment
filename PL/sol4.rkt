#lang racket
(provide (all-defined-out))
(define (check_bst l)
  (let ([parent (car l)]
        [left (car(cdr l))]
        [right (car(cdr(cdr l)))])
    (if (and (null? left) (null? right))
        #t
        (if (null? left)
            (< parent (car right))
            (if (null? right)
                (> parent (car left))
                (if (< parent (car left))
                    #f
                    (if (> parent (car right))
                        #f
                        (and (check_bst left)(check_bst right)))))))))
(define (apply f l)
  (if (null? l)
      '()
      (list (f (car l)) (apply f (car (cdr l))) (apply f (car(cdr (cdr l)))))))
