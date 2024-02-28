type nonrec file;

external makeFile: 'a => file = "%identity";

[@deriving jsProperties]
type nonrec uploadOptions = {
  uploadUrl: string,
  headers: {. "Accept": string},
  onError: string => unit,
  onSuccess: unit => unit,
  onProgress: (int, int) => unit,
};

type nonrec uploader;
[@mel.module "tus-js-client"] [@mel.new]
external jsUpload: (file, uploadOptions) => uploader = "Upload";

[@mel.send] external start: uploader => unit = "start";

let upload = (~file, ~uploadUrl, ~onSuccess, ~onError, ~onProgress) => {
  let headers = {"Accept": "application/vnd.vimeo.*+json;version=3.4"};
  let options =
    uploadOptions(~uploadUrl, ~headers, ~onSuccess, ~onError, ~onProgress);
  jsUpload(file, options)->start;
};
