load "$NCARG_ROOT/lib/ncarg/nclscripts/csm/gsn_code.ncl"
load "$NCARG_ROOT/lib/ncarg/nclscripts/csm/gsn_csm.ncl"

begin
;
; This grid comes from a FIM G4, curve=2 run.
;
  f = addfile("testicos.nc","r")

  x  = f->lon                    ; define the lon centers of the polygons
  y  = f->lat                    ; define the lat centers of the polygons
  cx = f->voronoi_vertex_lon     ; define the lon vertices of the icos polygons
  cy = f->voronoi_vertex_lat     ; define the lat vertices of the icos polygons
  ts2d = f->wZZZ(0,0,:)          ; select the 1st time and lvl idxs of the field "wZZZ"

  wks = gsn_open_wks("ncgm","geodesic")
;  gsn_define_colormap(wks,"gui_default")
;  gsn_define_colormap(wks,"MPL_gist_ncar")
;  gsn_define_colormap(wks,"circular_2")
;  gsn_define_colormap(wks,"default")
;  gsn_define_colormap(wks,"lithology")

  res                      = True
  res@gsnMaximize          = True           ; largest plot possible
  res@gsnSpreadColors      = True

; cant use CellFill due to "MeshScalarField", whatever that means
  res@cnFillMode           = "CellFill"
  res@cnCellFillEdgeColor  = "Black"

  res@cnFillOn             = True
;  res@cnFillMode           = "RasterFill"  ; setting this leads to the "black hole"
  res@cnLinesOn            = False          ; add contours to shading
;  res@cnLineLabelsOn       = True          ; label the contours

  res@lbLabelAutoStride    = True
;  res@lbBoxLinesOn         = True

;  res@mpProjection      = "Orthographic"
  res@mpProjection      = "CylindricalEqualArea"
  res@mpDataBaseVersion = "LowRes" ; resolution of continental boundaries
  res@mpCenterLatF      =  40     ; center latitude for display
  res@mpCenterLonF      = -100    ; center longitude for display
  res@mpGridAndLimbOn   = True    ; grid lines
  res@mpPerimOn         = False              ; turn off map perimeter

  res@sfXArray             = x
  res@sfYArray             = y
  res@sfXCellBounds        = cx
  res@sfYCellBounds        = cy

  res@tiMainString         = "Point distribution across MPI tasks"

  res@cnMaxLevelCount      = 64            ; number of contour levels
;  gsn_define_colormap(wks,"cb_9step")
  gsn_define_colormap(wks,"amwg256")
  map = gsn_csm_contour_map(wks,ts2d,res)

  ts2d = f->wZZZ(0,1,:)          ; select the 1st time and lvl idxs of the field "wZZZ"
  res@tiMainString         = "ipn values"
  res@cnMaxLevelCount      = 256            ; number of contour levels
  gsn_define_colormap(wks,"NCV_banded")
  map = gsn_csm_contour_map(wks,ts2d,res)

  ts2d = f->wZZZ(0,2,:)          ; select the 1st time and lvl idxs of the field "wZZZ"
  res@tiMainString         = "Number of sends for each grid point"

  gsn_define_colormap(wks,"gui_default")
  res@cnMaxLevelCount      = 5            ; number of contour levels
  map = gsn_csm_contour_map(wks,ts2d,res)

  ts2d = f->wZZZ(0,3,:)          ; select the 1st time and lvl idxs of the field "wZZZ"
  res@tiMainString         = "area normalized to smallest"
  res@cnMaxLevelCount      = 256            ; number of contour levels
  gsn_define_colormap(wks,"NCV_banded")
  map = gsn_csm_contour_map(wks,ts2d,res)

end
