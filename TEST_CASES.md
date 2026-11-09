# Riff House - Test Cases with Expected Output
Every transcript below was produced by running the real program (the lines after each prompt are what the tester typed; the big main-menu box is hidden to save space). Run the same keystrokes to reproduce them.
How to run a case yourself: `./riffhouse`, then type the values listed under *Keystrokes*, one per line; the final `0` exits.
Automated checks: the same scenarios (plus 40 sort runs, 250-product stress test and BST delete cases) were also run under AddressSanitizer + UndefinedBehaviorSanitizer with 65 automated checks, all passing, and no memory leak reports.

## TC-01 Adding products (valid input, bad prices, duplicate ID)

**What it tests:** Add a Fender accessory 'Test Drum' (invalid prices are rejected), then try to reuse existing ID 2001, then find the new product by keyword.

**Keystrokes (one per line, end with 0 to exit):** `4 | 1 | 3000 | Test Drum | 3 | 2 | abc | -5 | 99.5 | 20 | 4 | 1 | 2001 | 0 | 2 | 1 | drum`

**Expected result:** Product added; letters and negative prices rejected; duplicate ID refused; new product found.

**Actual output:**

```text
Enter choice (0-7): 4
--- Manage Products ---
  1. Add product
  2. Delete product
  3. Restock product
  0. Back
Choose: 1
New product ID (1-99999, 0 to cancel): 3000
Product name: Test Drum
Categories:
  1. Guitar
  2. Keyboard
  3. Accessories
Choose a category: 3
Partner brands:
  1. Yamaha
  2. Fender
  3. Roland
  4. Casio
Choose a brand: 2
Unit price in PHP (0.01 - 1000000): abc
  Invalid input. Enter a number from 0.01 to 1000000.00.
Unit price in PHP (0.01 - 1000000): -5
  Invalid input. Enter a number from 0.01 to 1000000.00.
Unit price in PHP (0.01 - 1000000): 99.5
Stock quantity (0 - 100000): 20
Added product 3000.
Enter choice (0-7): 4
--- Manage Products ---
  1. Add product
  2. Delete product
  3. Restock product
  0. Back
Choose: 1
New product ID (1-99999, 0 to cancel): 2001
  That ID already exists. Choose another.
New product ID (1-99999, 0 to cancel): 0
Cancelled.
Enter choice (0-7): 2
--- Search Products ---
  1. Keyword (name / category / brand)   [linear search]
  2. Product ID                          [linear search]
  3. Product ID                          [binary search on a sorted copy]
  4. Product ID                          [BST search]
  5. Compare all three on one ID
  0. Back
Choose: 1
Keyword: drum
ID    Name                              Category     Brand     Price(PHP)  Stock
--------------------------------------------------------------------------------
3000  Test Drum                         Accessories  Fender         99.50     20
1 match(es).
Enter choice (0-7): 0
Memory released -> list nodes: 15, BST nodes: 15, queue nodes: 0, stack nodes: 0
Thank you for using Riff House. Goodbye!
```

## TC-02 Linear search (keyword and ID)

**What it tests:** Keyword 'yamaha' (name, category and brand text are searched), then ID 2005 by linear search.

**Keystrokes (one per line, end with 0 to exit):** `2 | 1 | yamaha | 2 | 2 | 2005`

**Expected result:** 3 Yamaha products; ID 2005 is the 9th product in the list, so linear search needs 9 steps.

**Actual output:**

```text
Enter choice (0-7): 2
--- Search Products ---
  1. Keyword (name / category / brand)   [linear search]
  2. Product ID                          [linear search]
  3. Product ID                          [binary search on a sorted copy]
  4. Product ID                          [BST search]
  5. Compare all three on one ID
  0. Back
Choose: 1
Keyword: yamaha
ID    Name                              Category     Brand     Price(PHP)  Stock
--------------------------------------------------------------------------------
2003  Yamaha PAC112V Electric Guitar    Guitar       Yamaha      17500.00      5  LOW
2001  Yamaha F310 Acoustic Guitar       Guitar       Yamaha       8500.00     10
2002  Yamaha PSR-E373 Keyboard          Keyboard     Yamaha      11500.00      7
3 match(es).
Enter choice (0-7): 2
--- Search Products ---
  1. Keyword (name / category / brand)   [linear search]
  2. Product ID                          [linear search]
  3. Product ID                          [binary search on a sorted copy]
  4. Product ID                          [BST search]
  5. Compare all three on one ID
  0. Back
Choose: 2
Product ID: 2005
[Linear search] 9 step(s)
ID    Name                              Category     Brand     Price(PHP)  Stock
--------------------------------------------------------------------------------
2005  Fender CD-60S Acoustic Guitar     Guitar       Fender      13000.00      0  SOLD OUT
Enter choice (0-7): 0
Memory released -> list nodes: 14, BST nodes: 14, queue nodes: 0, stack nodes: 0
Thank you for using Riff House. Goodbye!
```

## TC-03 Binary search on sorted products

**What it tests:** ID 2005 and a missing ID with binary search. A temporary sorted copy is made first; the real inventory order is not changed.

**Keystrokes (one per line, end with 0 to exit):** `2 | 3 | 2005 | 2 | 3 | 9999`

**Expected result:** ID 2005 found in 3 steps (14 sorted items); missing ID 9999 reports 'Product not found.' after at most 4 steps.

**Actual output:**

```text
Enter choice (0-7): 2
--- Search Products ---
  1. Keyword (name / category / brand)   [linear search]
  2. Product ID                          [linear search]
  3. Product ID                          [binary search on a sorted copy]
  4. Product ID                          [BST search]
  5. Compare all three on one ID
  0. Back
Choose: 3
Product ID: 2005
[Binary search] 3 step(s)
ID    Name                              Category     Brand     Price(PHP)  Stock
--------------------------------------------------------------------------------
2005  Fender CD-60S Acoustic Guitar     Guitar       Fender      13000.00      0  SOLD OUT
Enter choice (0-7): 2
--- Search Products ---
  1. Keyword (name / category / brand)   [linear search]
  2. Product ID                          [linear search]
  3. Product ID                          [binary search on a sorted copy]
  4. Product ID                          [BST search]
  5. Compare all three on one ID
  0. Back
Choose: 3
Product ID: 9999
[Binary search] 4 step(s)
  Product not found.
Enter choice (0-7): 0
Memory released -> list nodes: 14, BST nodes: 14, queue nodes: 0, stack nodes: 0
Thank you for using Riff House. Goodbye!
```

## TC-04 Brand filtering

**What it tests:** Show only the Fender products.

**Keystrokes (one per line, end with 0 to exit):** `1 | 3 | 2`

**Expected result:** Only Fender rows (4 products).

**Actual output:**

```text
Enter choice (0-7): 1
--- Browse Products ---
  1. Display all products (inventory order)
  2. Display products by ID (BST in-order)
  3. Filter by partner brand
  0. Back
Choose: 3
Partner brands:
  1. Yamaha
  2. Fender
  3. Roland
  4. Casio
Choose a brand: 2
ID    Name                              Category     Brand     Price(PHP)  Stock
--------------------------------------------------------------------------------
2006  Fender Frontman 10G Amplifier     Accessories  Fender       3900.00     12
2005  Fender CD-60S Acoustic Guitar     Guitar       Fender      13000.00      0  SOLD OUT
2004  Fender Squier Stratocaster        Guitar       Fender      14500.00      8
2007  Fender 351 Guitar Picks (12pk)    Accessories  Fender        250.00     80
4 product(s) from Fender.
Enter choice (0-7): 0
Memory released -> list nodes: 14, BST nodes: 14, queue nodes: 0, stack nodes: 0
Thank you for using Riff House. Goodbye!
```

## TC-05 Sorting

**What it tests:** Merge sort by price, ascending (the other 4 algorithms and 4 sort keys are checked by the automated tests).

**Keystrokes (one per line, end with 0 to exit):** `3 | 4 | 1 | 1`

**Expected result:** 14 products ordered from the cheapest (250.00) to the most expensive (32000.00).

**Actual output:**

```text
Enter choice (0-7): 3
--- Sort Products ---
Algorithm:
  1. Bubble sort
  2. Selection sort
  3. Insertion sort
  4. Merge sort
  5. Quick sort
  0. Back
Choose: 4
Sort by:
  1. Price
  2. Stock quantity
  3. Name
  4. ID
Choose: 1
Order:
  1. Ascending
  2. Descending
Choose: 1
Merge sort by price (ascending):
ID    Name                              Category     Brand     Price(PHP)  Stock
--------------------------------------------------------------------------------
2007  Fender 351 Guitar Picks (12pk)    Accessories  Fender        250.00     80
2013  Casio SA-76 Mini Keyboard         Keyboard     Casio        2800.00     30
2006  Fender Frontman 10G Amplifier     Accessories  Fender       3900.00     12
2011  Casio CT-S300 Keyboard            Keyboard     Casio        6500.00     20
2001  Yamaha F310 Acoustic Guitar       Guitar       Yamaha       8500.00     10
2010  Roland Micro Cube GX Amp          Accessories  Roland       9500.00      9
2014  Casio LK-S250 Lighted Keyboard    Keyboard     Casio        9500.00      2  LOW
2002  Yamaha PSR-E373 Keyboard          Keyboard     Yamaha      11500.00      7
2005  Fender CD-60S Acoustic Guitar     Guitar       Fender      13000.00      0  SOLD OUT
2004  Fender Squier Stratocaster        Guitar       Fender      14500.00      8
2009  Roland GO:KEYS 3 Keyboard         Keyboard     Roland      15900.00      6
2003  Yamaha PAC112V Electric Guitar    Guitar       Yamaha      17500.00      5  LOW
2012  Casio CDP-S160 Digital Piano      Keyboard     Casio       21000.00      4  LOW
2008  Roland FP-10 Digital Piano        Keyboard     Roland      32000.00      3  LOW
Enter choice (0-7): 0
Memory released -> list nodes: 14, BST nodes: 14, queue nodes: 0, stack nodes: 0
Thank you for using Riff House. Goodbye!
```

## TC-06 Adding items to the cart

**What it tests:** Add 3 guitars and 1 keyboard, view the cart, try to add 8 more guitars (only 7 are left), add 2, then remove the keyboard.

**Keystrokes (one per line, end with 0 to exit):** `5 | 2 | 2001 | 3 | 5 | 2 | 2011 | 1 | 5 | 1 | 5 | 2 | 2001 | 8 | 2 | 5 | 3 | 2011 | 5 | 1`

**Expected result:** Quantities above stock are rejected by the prompt; the same product merges into one cart line (3 + 2 = 5).

**Actual output:**

```text
Enter choice (0-7): 5
--- Shopping Cart ---
  1. View cart
  2. Add product to cart
  3. Remove product from cart
  4. Empty the cart
  5. Checkout (place order in the queue)
  0. Back
Choose: 2
Product ID to add (0 to cancel): 2001
ID    Name                              Category     Brand     Price(PHP)  Stock
--------------------------------------------------------------------------------
2001  Yamaha F310 Acoustic Guitar       Guitar       Yamaha       8500.00     10
Quantity (1 - 10): 3
Added 3 x Yamaha F310 Acoustic Guitar to the cart.
Enter choice (0-7): 5
--- Shopping Cart ---
  1. View cart
  2. Add product to cart
  3. Remove product from cart
  4. Empty the cart
  5. Checkout (place order in the queue)
  0. Back
Choose: 2
Product ID to add (0 to cancel): 2011
ID    Name                              Category     Brand     Price(PHP)  Stock
--------------------------------------------------------------------------------
2011  Casio CT-S300 Keyboard            Keyboard     Casio        6500.00     20
Quantity (1 - 20): 1
Added 1 x Casio CT-S300 Keyboard to the cart.
Enter choice (0-7): 5
--- Shopping Cart ---
  1. View cart
  2. Add product to cart
  3. Remove product from cart
  4. Empty the cart
  5. Checkout (place order in the queue)
  0. Back
Choose: 1
ID    Name                                Qty    Unit(PHP)      Subtotal
------------------------------------------------------------------------
2001  Yamaha F310 Acoustic Guitar           3      8500.00      25500.00
2011  Casio CT-S300 Keyboard                1      6500.00       6500.00
------------------------------------------------------------------------
                                             TOTAL (PHP):       32000.00
Enter choice (0-7): 5
--- Shopping Cart ---
  1. View cart
  2. Add product to cart
  3. Remove product from cart
  4. Empty the cart
  5. Checkout (place order in the queue)
  0. Back
Choose: 2
Product ID to add (0 to cancel): 2001
ID    Name                              Category     Brand     Price(PHP)  Stock
--------------------------------------------------------------------------------
2001  Yamaha F310 Acoustic Guitar       Guitar       Yamaha       8500.00     10
Quantity (1 - 7): 8
  Invalid input. Enter a whole number from 1 to 7.
Quantity (1 - 7): 2
Added 2 x Yamaha F310 Acoustic Guitar to the cart.
Enter choice (0-7): 5
--- Shopping Cart ---
  1. View cart
  2. Add product to cart
  3. Remove product from cart
  4. Empty the cart
  5. Checkout (place order in the queue)
  0. Back
Choose: 3
Product ID to remove (0 to cancel): 2011
Removed product 2011 from the cart.
Enter choice (0-7): 5
--- Shopping Cart ---
  1. View cart
  2. Add product to cart
  3. Remove product from cart
  4. Empty the cart
  5. Checkout (place order in the queue)
  0. Back
Choose: 1
ID    Name                                Qty    Unit(PHP)      Subtotal
------------------------------------------------------------------------
2001  Yamaha F310 Acoustic Guitar           5      8500.00      42500.00
------------------------------------------------------------------------
                                             TOTAL (PHP):       42500.00
Enter choice (0-7): 0
Memory released -> list nodes: 14, BST nodes: 14, queue nodes: 0, stack nodes: 0
Thank you for using Riff House. Goodbye!
```

## TC-07 Checkout (cart becomes a queued order)

**What it tests:** Customer Justine checks out a cart, then the order queue is viewed.

**Keystrokes (one per line, end with 0 to exit):** `5 | 2 | 2001 | 2 | 5 | 2 | 2011 | 1 | 5 | 5 | y | Justine | 6 | 1`

**Expected result:** Order #1 is placed at position 1 and the cart is emptied.

**Actual output:**

```text
Enter choice (0-7): 5
--- Shopping Cart ---
  1. View cart
  2. Add product to cart
  3. Remove product from cart
  4. Empty the cart
  5. Checkout (place order in the queue)
  0. Back
Choose: 2
Product ID to add (0 to cancel): 2001
ID    Name                              Category     Brand     Price(PHP)  Stock
--------------------------------------------------------------------------------
2001  Yamaha F310 Acoustic Guitar       Guitar       Yamaha       8500.00     10
Quantity (1 - 10): 2
Added 2 x Yamaha F310 Acoustic Guitar to the cart.
Enter choice (0-7): 5
--- Shopping Cart ---
  1. View cart
  2. Add product to cart
  3. Remove product from cart
  4. Empty the cart
  5. Checkout (place order in the queue)
  0. Back
Choose: 2
Product ID to add (0 to cancel): 2011
ID    Name                              Category     Brand     Price(PHP)  Stock
--------------------------------------------------------------------------------
2011  Casio CT-S300 Keyboard            Keyboard     Casio        6500.00     20
Quantity (1 - 20): 1
Added 1 x Casio CT-S300 Keyboard to the cart.
Enter choice (0-7): 5
--- Shopping Cart ---
  1. View cart
  2. Add product to cart
  3. Remove product from cart
  4. Empty the cart
  5. Checkout (place order in the queue)
  0. Back
Choose: 5
ID    Name                                Qty    Unit(PHP)      Subtotal
------------------------------------------------------------------------
2001  Yamaha F310 Acoustic Guitar           2      8500.00      17000.00
2011  Casio CT-S300 Keyboard                1      6500.00       6500.00
------------------------------------------------------------------------
                                             TOTAL (PHP):       23500.00
Place this order? (y/n): y
Customer name: Justine
Order #1 placed for Justine. Position in queue: 1.
Enter choice (0-7): 6
--- Order Queue ---
  1. View waiting orders
  2. Process next order
  0. Back
Choose: 1
Order queue (1 waiting), next to be served first:
  #1  Order 1 - Justine: [ID 2001 x2] [ID 2011 x1]
Enter choice (0-7): 0
Memory released -> list nodes: 14, BST nodes: 14, queue nodes: 1, stack nodes: 0
Thank you for using Riff House. Goodbye!
```

## TC-08 Updating stock when an order is processed

**What it tests:** Process Justine's order, then look at the stock of the two products.

**Keystrokes (one per line, end with 0 to exit):** `5 | 2 | 2001 | 2 | 5 | 2 | 2011 | 1 | 5 | 5 | y | Justine | 6 | 2 | 2 | 1 | F310 | 2 | 1 | CT-S300`

**Expected result:** Stock drops: 2001 from 10 to 8, 2011 from 20 to 19; a transaction receipt is printed (2 x 8500.00 + 6500.00 = 23500.00).

**Actual output:**

```text
Enter choice (0-7): 5
--- Shopping Cart ---
  1. View cart
  2. Add product to cart
  3. Remove product from cart
  4. Empty the cart
  5. Checkout (place order in the queue)
  0. Back
Choose: 2
Product ID to add (0 to cancel): 2001
ID    Name                              Category     Brand     Price(PHP)  Stock
--------------------------------------------------------------------------------
2001  Yamaha F310 Acoustic Guitar       Guitar       Yamaha       8500.00     10
Quantity (1 - 10): 2
Added 2 x Yamaha F310 Acoustic Guitar to the cart.
Enter choice (0-7): 5
--- Shopping Cart ---
  1. View cart
  2. Add product to cart
  3. Remove product from cart
  4. Empty the cart
  5. Checkout (place order in the queue)
  0. Back
Choose: 2
Product ID to add (0 to cancel): 2011
ID    Name                              Category     Brand     Price(PHP)  Stock
--------------------------------------------------------------------------------
2011  Casio CT-S300 Keyboard            Keyboard     Casio        6500.00     20
Quantity (1 - 20): 1
Added 1 x Casio CT-S300 Keyboard to the cart.
Enter choice (0-7): 5
--- Shopping Cart ---
  1. View cart
  2. Add product to cart
  3. Remove product from cart
  4. Empty the cart
  5. Checkout (place order in the queue)
  0. Back
Choose: 5
ID    Name                                Qty    Unit(PHP)      Subtotal
------------------------------------------------------------------------
2001  Yamaha F310 Acoustic Guitar           2      8500.00      17000.00
2011  Casio CT-S300 Keyboard                1      6500.00       6500.00
------------------------------------------------------------------------
                                             TOTAL (PHP):       23500.00
Place this order? (y/n): y
Customer name: Justine
Order #1 placed for Justine. Position in queue: 1.
Enter choice (0-7): 6
--- Order Queue ---
  1. View waiting orders
  2. Process next order
  0. Back
Choose: 2
Order #1 served.
  Transaction #1 (Order #1) - Justine
      2 x Yamaha F310 Acoustic Guitar @ PHP 8500.00 = PHP 17000.00
      1 x Casio CT-S300 Keyboard @ PHP 6500.00 = PHP 6500.00
      TOTAL: PHP 23500.00
Enter choice (0-7): 2
--- Search Products ---
  1. Keyword (name / category / brand)   [linear search]
  2. Product ID                          [linear search]
  3. Product ID                          [binary search on a sorted copy]
  4. Product ID                          [BST search]
  5. Compare all three on one ID
  0. Back
Choose: 1
Keyword: F310
ID    Name                              Category     Brand     Price(PHP)  Stock
--------------------------------------------------------------------------------
2001  Yamaha F310 Acoustic Guitar       Guitar       Yamaha       8500.00      8
1 match(es).
Enter choice (0-7): 2
--- Search Products ---
  1. Keyword (name / category / brand)   [linear search]
  2. Product ID                          [linear search]
  3. Product ID                          [binary search on a sorted copy]
  4. Product ID                          [BST search]
  5. Compare all three on one ID
  0. Back
Choose: 1
Keyword: CT-S300
ID    Name                              Category     Brand     Price(PHP)  Stock
--------------------------------------------------------------------------------
2011  Casio CT-S300 Keyboard            Keyboard     Casio        6500.00     19
1 match(es).
Enter choice (0-7): 0
Memory released -> list nodes: 14, BST nodes: 14, queue nodes: 0, stack nodes: 1
Thank you for using Riff House. Goodbye!
```

## TC-09 Order queue (first come, first served)

**What it tests:** Justine orders first, Ana second. Process one order and view the queue again.

**Keystrokes (one per line, end with 0 to exit):** `5 | 2 | 2001 | 1 | 5 | 5 | y | Justine | 5 | 2 | 2002 | 1 | 5 | 5 | y | Ana | 6 | 1 | 6 | 2 | 6 | 1`

**Expected result:** Justine (Order #1) is served first; Ana stays in the queue.

**Actual output:**

```text
Enter choice (0-7): 5
--- Shopping Cart ---
  1. View cart
  2. Add product to cart
  3. Remove product from cart
  4. Empty the cart
  5. Checkout (place order in the queue)
  0. Back
Choose: 2
Product ID to add (0 to cancel): 2001
ID    Name                              Category     Brand     Price(PHP)  Stock
--------------------------------------------------------------------------------
2001  Yamaha F310 Acoustic Guitar       Guitar       Yamaha       8500.00     10
Quantity (1 - 10): 1
Added 1 x Yamaha F310 Acoustic Guitar to the cart.
Enter choice (0-7): 5
--- Shopping Cart ---
  1. View cart
  2. Add product to cart
  3. Remove product from cart
  4. Empty the cart
  5. Checkout (place order in the queue)
  0. Back
Choose: 5
ID    Name                                Qty    Unit(PHP)      Subtotal
------------------------------------------------------------------------
2001  Yamaha F310 Acoustic Guitar           1      8500.00       8500.00
------------------------------------------------------------------------
                                             TOTAL (PHP):        8500.00
Place this order? (y/n): y
Customer name: Justine
Order #1 placed for Justine. Position in queue: 1.
Enter choice (0-7): 5
--- Shopping Cart ---
  1. View cart
  2. Add product to cart
  3. Remove product from cart
  4. Empty the cart
  5. Checkout (place order in the queue)
  0. Back
Choose: 2
Product ID to add (0 to cancel): 2002
ID    Name                              Category     Brand     Price(PHP)  Stock
--------------------------------------------------------------------------------
2002  Yamaha PSR-E373 Keyboard          Keyboard     Yamaha      11500.00      7
Quantity (1 - 7): 1
Added 1 x Yamaha PSR-E373 Keyboard to the cart.
Enter choice (0-7): 5
--- Shopping Cart ---
  1. View cart
  2. Add product to cart
  3. Remove product from cart
  4. Empty the cart
  5. Checkout (place order in the queue)
  0. Back
Choose: 5
ID    Name                                Qty    Unit(PHP)      Subtotal
------------------------------------------------------------------------
2002  Yamaha PSR-E373 Keyboard              1     11500.00      11500.00
------------------------------------------------------------------------
                                             TOTAL (PHP):       11500.00
Place this order? (y/n): y
Customer name: Ana
Order #2 placed for Ana. Position in queue: 2.
Enter choice (0-7): 6
--- Order Queue ---
  1. View waiting orders
  2. Process next order
  0. Back
Choose: 1
Order queue (2 waiting), next to be served first:
  #1  Order 1 - Justine: [ID 2001 x1]
  #2  Order 2 - Ana: [ID 2002 x1]
Enter choice (0-7): 6
--- Order Queue ---
  1. View waiting orders
  2. Process next order
  0. Back
Choose: 2
Order #1 served.
  Transaction #1 (Order #1) - Justine
      1 x Yamaha F310 Acoustic Guitar @ PHP 8500.00 = PHP 8500.00
      TOTAL: PHP 8500.00
Enter choice (0-7): 6
--- Order Queue ---
  1. View waiting orders
  2. Process next order
  0. Back
Choose: 1
Order queue (1 waiting), next to be served first:
  #1  Order 2 - Ana: [ID 2002 x1]
Enter choice (0-7): 0
Memory released -> list nodes: 14, BST nodes: 14, queue nodes: 1, stack nodes: 1
Thank you for using Riff House. Goodbye!
```

## TC-10 Transaction history and refund

**What it tests:** Serve an order, view the history, refund the last transaction, view the history again.

**Keystrokes (one per line, end with 0 to exit):** `5 | 2 | 2001 | 2 | 5 | 5 | y | Justine | 6 | 2 | 7 | 1 | 7 | 2 | 7 | 1`

**Expected result:** History shows Transaction #1; the refund puts 2 guitars back in stock; history is then empty.

**Actual output:**

```text
Enter choice (0-7): 5
--- Shopping Cart ---
  1. View cart
  2. Add product to cart
  3. Remove product from cart
  4. Empty the cart
  5. Checkout (place order in the queue)
  0. Back
Choose: 2
Product ID to add (0 to cancel): 2001
ID    Name                              Category     Brand     Price(PHP)  Stock
--------------------------------------------------------------------------------
2001  Yamaha F310 Acoustic Guitar       Guitar       Yamaha       8500.00     10
Quantity (1 - 10): 2
Added 2 x Yamaha F310 Acoustic Guitar to the cart.
Enter choice (0-7): 5
--- Shopping Cart ---
  1. View cart
  2. Add product to cart
  3. Remove product from cart
  4. Empty the cart
  5. Checkout (place order in the queue)
  0. Back
Choose: 5
ID    Name                                Qty    Unit(PHP)      Subtotal
------------------------------------------------------------------------
2001  Yamaha F310 Acoustic Guitar           2      8500.00      17000.00
------------------------------------------------------------------------
                                             TOTAL (PHP):       17000.00
Place this order? (y/n): y
Customer name: Justine
Order #1 placed for Justine. Position in queue: 1.
Enter choice (0-7): 6
--- Order Queue ---
  1. View waiting orders
  2. Process next order
  0. Back
Choose: 2
Order #1 served.
  Transaction #1 (Order #1) - Justine
      2 x Yamaha F310 Acoustic Guitar @ PHP 8500.00 = PHP 17000.00
      TOTAL: PHP 17000.00
Enter choice (0-7): 7
--- Transaction History ---
  1. View transactions (newest first)
  2. Refund the last transaction
  0. Back
Choose: 1
Transaction history (1), newest first:
  Transaction #1 (Order #1) - Justine
      2 x Yamaha F310 Acoustic Guitar @ PHP 8500.00 = PHP 17000.00
      TOTAL: PHP 17000.00
Enter choice (0-7): 7
--- Transaction History ---
  1. View transactions (newest first)
  2. Refund the last transaction
  0. Back
Choose: 2
Refunding transaction #1 for Justine:
  Returned 2 x Yamaha F310 Acoustic Guitar to stock.
Refund total: PHP 17000.00
Enter choice (0-7): 7
--- Transaction History ---
  1. View transactions (newest first)
  2. Refund the last transaction
  0. Back
Choose: 1
No transactions yet.
Enter choice (0-7): 0
Memory released -> list nodes: 14, BST nodes: 14, queue nodes: 0, stack nodes: 0
Thank you for using Riff House. Goodbye!
```

## TC-11 Invalid inputs

**What it tests:** Letters, out-of-range numbers and blanks in menus; an unknown product ID; a sold-out product; quantity 0.

**Keystrokes (one per line, end with 0 to exit):** `abc | 99 | <Enter> | 2 | x | 9 | 0 | 5 | 2 | 9999 | 5 | 2 | 2005 | 5 | 2 | 2001 | 0 | 1`

**Expected result:** Every bad entry is rejected with a message and the program keeps running.

**Actual output:**

```text
Enter choice (0-7): abc
  Invalid input. Enter a whole number from 0 to 7.
Enter choice (0-7): 99
  Invalid input. Enter a whole number from 0 to 7.
Enter choice (0-7): 
  Invalid input. Enter a whole number from 0 to 7.
Enter choice (0-7): 2
--- Search Products ---
  1. Keyword (name / category / brand)   [linear search]
  2. Product ID                          [linear search]
  3. Product ID                          [binary search on a sorted copy]
  4. Product ID                          [BST search]
  5. Compare all three on one ID
  0. Back
Choose: x
  Invalid input. Enter a whole number from 0 to 5.
Choose: 9
  Invalid input. Enter a whole number from 0 to 5.
Choose: 0
Enter choice (0-7): 5
--- Shopping Cart ---
  1. View cart
  2. Add product to cart
  3. Remove product from cart
  4. Empty the cart
  5. Checkout (place order in the queue)
  0. Back
Choose: 2
Product ID to add (0 to cancel): 9999
Product ID 9999 not found.
Enter choice (0-7): 5
--- Shopping Cart ---
  1. View cart
  2. Add product to cart
  3. Remove product from cart
  4. Empty the cart
  5. Checkout (place order in the queue)
  0. Back
Choose: 2
Product ID to add (0 to cancel): 2005
Fender CD-60S Acoustic Guitar is not available (sold out, reserved by waiting orders, or all already in your cart).
Enter choice (0-7): 5
--- Shopping Cart ---
  1. View cart
  2. Add product to cart
  3. Remove product from cart
  4. Empty the cart
  5. Checkout (place order in the queue)
  0. Back
Choose: 2
Product ID to add (0 to cancel): 2001
ID    Name                              Category     Brand     Price(PHP)  Stock
--------------------------------------------------------------------------------
2001  Yamaha F310 Acoustic Guitar       Guitar       Yamaha       8500.00     10
Quantity (1 - 10): 0
  Invalid input. Enter a whole number from 1 to 10.
Quantity (1 - 10): 1
Added 1 x Yamaha F310 Acoustic Guitar to the cart.
Enter choice (0-7): 0
Memory released -> list nodes: 14, BST nodes: 14, queue nodes: 0, stack nodes: 0
Thank you for using Riff House. Goodbye!
```

## TC-12 Empty collections

**What it tests:** On a fresh start: view the empty queue, process an order, view/refund an empty history, view and checkout an empty cart.

**Keystrokes (one per line, end with 0 to exit):** `6 | 1 | 6 | 2 | 7 | 1 | 7 | 2 | 5 | 1 | 5 | 5`

**Expected result:** Each screen answers with an 'empty' message instead of failing.

**Actual output:**

```text
Enter choice (0-7): 6
--- Order Queue ---
  1. View waiting orders
  2. Process next order
  0. Back
Choose: 1
The order queue is empty.
Enter choice (0-7): 6
--- Order Queue ---
  1. View waiting orders
  2. Process next order
  0. Back
Choose: 2
The order queue is empty.
Enter choice (0-7): 7
--- Transaction History ---
  1. View transactions (newest first)
  2. Refund the last transaction
  0. Back
Choose: 1
No transactions yet.
Enter choice (0-7): 7
--- Transaction History ---
  1. View transactions (newest first)
  2. Refund the last transaction
  0. Back
Choose: 2
No transaction to refund.
Enter choice (0-7): 5
--- Shopping Cart ---
  1. View cart
  2. Add product to cart
  3. Remove product from cart
  4. Empty the cart
  5. Checkout (place order in the queue)
  0. Back
Choose: 1
The cart is empty.
Enter choice (0-7): 5
--- Shopping Cart ---
  1. View cart
  2. Add product to cart
  3. Remove product from cart
  4. Empty the cart
  5. Checkout (place order in the queue)
  0. Back
Choose: 5
The cart is empty. Add products before checkout.
Enter choice (0-7): 0
Memory released -> list nodes: 14, BST nodes: 14, queue nodes: 0, stack nodes: 0
Thank you for using Riff House. Goodbye!
```

## TC-13 Delete protection and memory release

**What it tests:** Ana queues 2 guitars, then someone tries to delete that product; the program is exited with the order still waiting.

**Keystrokes (one per line, end with 0 to exit):** `5 | 2 | 2001 | 2 | 5 | 5 | y | Ana | 4 | 2 | 2001`

**Expected result:** Delete is refused while the product is reserved; on exit every dynamic node is freed (the waiting order's queue node is released too).

**Actual output:**

```text
Enter choice (0-7): 5
--- Shopping Cart ---
  1. View cart
  2. Add product to cart
  3. Remove product from cart
  4. Empty the cart
  5. Checkout (place order in the queue)
  0. Back
Choose: 2
Product ID to add (0 to cancel): 2001
ID    Name                              Category     Brand     Price(PHP)  Stock
--------------------------------------------------------------------------------
2001  Yamaha F310 Acoustic Guitar       Guitar       Yamaha       8500.00     10
Quantity (1 - 10): 2
Added 2 x Yamaha F310 Acoustic Guitar to the cart.
Enter choice (0-7): 5
--- Shopping Cart ---
  1. View cart
  2. Add product to cart
  3. Remove product from cart
  4. Empty the cart
  5. Checkout (place order in the queue)
  0. Back
Choose: 5
ID    Name                                Qty    Unit(PHP)      Subtotal
------------------------------------------------------------------------
2001  Yamaha F310 Acoustic Guitar           2      8500.00      17000.00
------------------------------------------------------------------------
                                             TOTAL (PHP):       17000.00
Place this order? (y/n): y
Customer name: Ana
Order #1 placed for Ana. Position in queue: 1.
Enter choice (0-7): 4
--- Manage Products ---
  1. Add product
  2. Delete product
  3. Restock product
  0. Back
Choose: 2
Product ID to delete (0 to cancel): 2001
Cannot delete: 2 unit(s) are waiting in the order queue.
Enter choice (0-7): 0
Memory released -> list nodes: 14, BST nodes: 14, queue nodes: 1, stack nodes: 0
Thank you for using Riff House. Goodbye!
```
