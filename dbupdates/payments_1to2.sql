DELETE FROM payments;
ALTER TABLE payments ADD blockNumber INTEGER DEFAULT 0;
ALTER TABLE payments ADD type INTEGER DEFAULT 0;
DROP INDEX paymentsUniqueIdx;
CREATE UNIQUE INDEX paymentsUniqueIdx ON payments ( currency ASC, address ASC, txid ASC, isInput ASC, blockNumber ASC );
