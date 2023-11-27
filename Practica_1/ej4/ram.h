// Single-header environment to program a Random-Access Machine (RAM).

#include <vector>

// Type of register indices.

typedef unsigned int index;

// Type of register and memory contents.

typedef long int integer;	// Ideally, they should be unbounded.

// Registers and memory.

typedef std::vector<integer> registers, memory;

// External declarations.

extern registers r;		// The number of registers is finite.
extern memory m;		// Ideally, memory should be infinite.

// Machine initialization with a finite number of registers.

inline void initialize(index n) { r.resize(n, 0); }

// Instrucion set:
//
// reset(i)        ▶ r[i] ← 0
// inc(i)          ▶ r[i] ← r[i] + 1
// dec(i)          ▶ r[i] ← r[i] − 1
// load(i, j)      ▶ r[i] ← m[r[j]]
// store(i, j)     ▶ m[r[j]] ← r[i]
// cgoto(i, l)     ▶ if r[i] ≥ 0 then pc ← l

inline void reset(index i)          { r.at(i) = 0; }
inline void inc(index i)            { ++r.at(i); }
inline void dec(index i)            { --r.at(i); }
inline void load(index i, index j)  { r.at(i) = m.at(r.at(j)); }
inline void store(index i, index j) { m.at(r.at(j)) = r.at(i); }

// This cannot be a function because of goto labels.

#define cgoto(i, l)                 { if (r.at(i) >= 0) goto l; }

// Plus the "too powerful" multiplication instruction:
//
// mul(i, j)       ▶ r[i] ← r[i]·r[j]

inline void mul(index i, index j)   { r.at(i) *= r.at(j); }
