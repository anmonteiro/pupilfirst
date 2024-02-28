type nonrec t =
  | Reloading(array(bool))
  | LoadingMore;
let setReloading = t =>
  switch (t) {
  | LoadingMore => Reloading([|true|])
  | Reloading(times) =>
    times->(Js.Array.push(~value=true))->ignore;
    Reloading(times);
  };
let setNotLoading = t =>
  switch (t) {
  | LoadingMore => Reloading([||])
  | Reloading(times) =>
    times->Js.Array.pop->ignore;
    Reloading(times);
  };
let empty = () => Reloading([||]);
