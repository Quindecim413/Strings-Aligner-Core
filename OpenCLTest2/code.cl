__kernel void makeNWScore(
    __global const int *a_g, __global const int *b_g, 
    __global int *prev, __global int *curr, __global int *next, 
    __constant int *s, int alphabet_size, 
    __global int *res, 
    int len_a, int len_b,
    int d, int start, int iter_num)
{
  int gid = get_global_id(0);
  int pos = start + gid;
  int _a = a_g[iter_num + 1 - pos];
  int _b = b_g[pos - 1];
  int similarity = s[_a*alphabet_size + _b];

  int _match = prev[pos-1] + similarity;
  int _delete = curr[pos] + d;
  int _insert = curr[pos-1] + d;

  int _max = _match > _delete ? _match: _delete;
  _max = _max > _insert? _max: _insert;

  next[pos] = _max;
  if(gid == 0){
      if (iter_num < len_a - 1){
          next[0] = (iter_num + 2) * d;
      }
      if (iter_num < len_b - 1){
          next[iter_num + 2] = (iter_num + 2) * d;
      }
      if (iter_num + 1 >= len_a){
          res[iter_num + 2 - len_a] = next[pos];
      }
  }
}