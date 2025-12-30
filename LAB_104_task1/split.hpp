#include "sequence.hpp"
/**
 * @brief Splits a sequence into two output sequences by alternating blocks of elements,
 *        starting from a given key occurrence, removing the moved elements from the source.
 *
 * Starting at the position of the start_occ-th occurrence of start_key (or at position 0 if
 * start_occ == 0), this function performs up to count iterations of the following:
 *  - take up to len1 consecutive elements from the current position, append them to seq1,
 *    and remove them from seq;
 *  - then take up to len2 consecutive elements from the (new) current position, append them
 *    to seq2, and remove them from seq.
 *
 * The current position does not advance between moves because elements are removed from seq;
 * after each removal the next element shifts into the same index. Iteration stops early if
 * the source sequence is exhausted.
 *
 * @tparam Key  Key type stored in the sequence.
 * @tparam Info Associated information type stored in the sequence.
 *
 * @param seq       The source sequence to split. This parameter is modified: elements moved
 *                  to seq1/seq2 are removed from seq. WARNING: this parameter in not constant as in the regulation, because we need to remove elements from it.
 * @param start_key Key value used to locate the starting position.
 * @param start_occ 0-based behaviour: if start_occ == 0, splitting begins at index 0. If
 *                  start_occ > 0, the function locates the start_occ-th occurrence of
 *                  start_key (1st occurrence when start_occ == 1, etc.) and begins there.
 * @param len1      Maximum number of consecutive elements to move to seq1 during each iteration.
 * @param len2      Maximum number of consecutive elements to move to seq2 during each iteration.
 * @param count     Number of alternating (len1, len2) blocks to perform. The operation may
 *                  terminate earlier if seq is exhausted.
 * @param seq1      Output sequence receiving the first block of each alternation (append-only).
 * @param seq2      Output sequence receiving the second block of each alternation (append-only).
 *
 * @throws std::invalid_argument if start_occ < 0, len1 < 0, len2 < 0, count < 0, or count > seq.size().
 * @throws std::invalid_argument if a positive start_occ is provided but the requested occurrence
 *                              of start_key cannot be found in seq.
 *
 * @note
 * - seq1 and seq2 are appended to; their existing contents are preserved.
 * - Because elements are removed from seq as they are transferred, indices of subsequent
 *   elements shift left; this function relies on that shifting behavior and always reads
 *   from the current index.
 * - The function has no return value; resulting partitions are visible in seq1, seq2, and seq.
 */
template <typename Key, typename Info>
void split_pos(Sequence <Key, Info>& seq, int start_pos, int len1, int len2, int count, Sequence <Key, Info>& seq1, Sequence <Key, Info>& seq2) {

    if (start_pos < 0 || start_pos > seq.size() || len1 < 0 || len2 < 0 || count < 0 || count > seq.size()) {
        throw std::invalid_argument("Invalid argument");
    }

    int currentPos = start_pos;

    while(start_pos < seq.size() && count > 0){
        for(int i=0; i<len1 && start_pos < seq.size(); i++){
            seq1.push_back(seq.get_key_at(start_pos), seq.get_info_at(start_pos));
            //std::cout<< "Added to seq1: " << seq.get_key_at(start_pos) << ", " << seq.get_info_at(start_pos) << std::endl;
            seq.remove_at(start_pos);
        }
        for(int j=0; j<len2 && start_pos < seq.size(); j++){
            seq2.push_back(seq.get_key_at(start_pos), seq.get_info_at(start_pos));
            //std::cout<< "Added to seq2: " << seq.get_key_at(start_pos) << ", " << seq.get_info_at(start_pos) << std::endl;
            seq.remove_at(start_pos);
        }
        count--;
    }
}

/**
 * @brief Splits a sequence `seq` into two new sequences (`seq1` and `seq2`)
 *        based on a starting key, segment lengths, and a repetition count.
 *
 * The function searches the given sequence `seq` for the specified `start_key`.
 * Once the `start_occ`-th occurrence of this key is found, it begins alternatingly
 * copying elements from the original sequence into `seq1` and `seq2`, in blocks
 * of lengths `len1` and `len2`, respectively. This alternating copy process is
 * repeated `count` times.
 *
 * Each element that is copied into either `seq1` or `seq2` is simultaneously
 * removed from the original sequence `seq`.
 *
 * @tparam Key   Type of key used in the sequence.
 * @tparam Info  Type of associated information stored in the sequence.
 *
 * @param seq         The original sequence to be split.
 * @param start_key   The key from which to start splitting.
 * @param start_occ   The occurrence number of `start_key` to begin from (e.g., 1 = first occurrence).
 * @param len1        Number of elements to copy into `seq1` during each iteration.
 * @param len2        Number of elements to copy into `seq2` during each iteration.
 * @param count       Number of alternating copy cycles (each consisting of len1 + len2 elements).
 * @param seq1        Output sequence that receives the first segments (len1 elements per cycle).
 * @param seq2        Output sequence that receives the second segments (len2 elements per cycle).
 *
 * @throws std::invalid_argument if:
 *         - any of `start_occ`, `len1`, `len2`, or `count` is negative,
 *         - `count` exceeds the size of `seq`,
 *         - the `start_key` is not found `start_occ` times in the sequence.
 *
 * @note 
 *  - This function assumes that the `Sequence` class provides the following methods:
 *      - `size()` — returns the number of elements,
 *      - `get_key_at(i)` and `get_info_at(i)` — return the key and value at index `i`,
 *      - `push_back(key, info)` — appends a new element to the sequence,
 *      - `remove_at(i)` — removes the element at index `i`.
 *  - Removing elements shifts subsequent indices, so care is taken to handle indexing correctly.
 *
 * @example
 * Sequence<char, int> seq, seq1, seq2;
 * // seq = [A, B, C, A, D, E, F, A, G]
 * split_key(seq, 'A', 2, 2, 1, 2, seq1, seq2);
 * // seq1 and seq2 now contain alternating chunks taken from seq.
 */

template <typename Key, typename Info>
void split_key(Sequence <Key, Info>& seq, const Key& start_key, int start_occ, int len1, int len2, int count, Sequence <Key, Info>& seq1, Sequence <Key, Info>& seq2) {

    if (start_occ < 0 || len1 < 0 || len2 < 0 || count < 0 || count > seq.size()) {
        throw std::invalid_argument("Invalid argument");
    }
        int currentPos = 0;

    while(start_occ > 0 && seq.size() > 0){
        for(int i=0; i<seq.size(); i++){
            if(seq.get_key_at(i) == start_key){
                start_occ--;
                if(start_occ == 0){
                    currentPos = i;
                    break;
                }
            }
        }
        if(start_occ > 0){
            throw std::invalid_argument("Key occurrence not found");
        }
    }

    while(currentPos < seq.size() && count > 0){
        for(int i=0; i<len1 && currentPos < seq.size(); i++){
            seq1.push_back(seq.get_key_at(currentPos), seq.get_info_at(currentPos));
            //std::cout<< "Added to seq1: " << seq.get_key_at(currentPos) << ", " << seq.get_info_at(currentPos) << std::endl;
            seq.remove_at(currentPos);
        }
        for(int j=0; j<len2 && currentPos < seq.size(); j++){
            seq2.push_back(seq.get_key_at(currentPos), seq.get_info_at(currentPos));
            //std::cout<< "Added to seq2: " << seq.get_key_at(currentPos) << ", " << seq.get_info_at(currentPos) << std::endl;
            seq.remove_at(currentPos);
        }
        count--;
    }
}