// -------------------------------------------------------------------------- -
// Copyright (c) 2016 Andrew Mac
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#include "wxTorqueAssetTree.h"

#include <plugins/plugins_shared.h>
#include "scene/components/animationComponent.h"
#include "scene/components/controllerComponent.h"
#include "scene/components/lighting/lightComponent.h"
#include "scene/components/meshComponent.h"
#include "scene/components/physics/physicsBoxComponent.h"
#include "scene/components/physics/physicsSphereComponent.h"
#include "scene/components/textComponent.h"
#include "module/moduleManager.h"
#include "materials/materialAsset.h"

IMPLEMENT_DYNAMIC_CLASS(wxTorqueAssetTree, wxTreeCtrl)

BEGIN_EVENT_TABLE(wxTorqueAssetTree, wxTreeCtrl)

END_EVENT_TABLE()

void wxTorqueAssetTree::Init()
{
   SetImageList(mEditorManager->mCommonIcons);
}

void wxTorqueAssetTree::ShowAssets(const char* filter)
{
   bool performFilter = false;
   if (filter != NULL && dStrlen(filter) > 0)
      performFilter = true;

   // Clear list.
   DeleteAllItems();
   wxTreeItemId mAssetListRoot = AddRoot("ROOT");

   Vector<ModuleInfo>* modules = mEditorManager->getModuleList();

   const char* currentModuleID = "";
   wxTreeItemId currentModuleTreeID = mAssetListRoot;
   for (Vector<ModuleInfo>::iterator modulesItr = modules->begin(); modulesItr != modules->end(); ++modulesItr)
   {
      if (dStrcmp(modulesItr->moduleID, currentModuleID) != 0)
      {
         currentModuleID = modulesItr->moduleID;
         currentModuleTreeID = AppendItem(mAssetListRoot, modulesItr->moduleID, 0, -1, new ModuleTreeItemData(*modulesItr));
      }

      for (Vector<AssetCategoryInfo>::iterator assetCatItr = modulesItr->assets.begin(); assetCatItr != modulesItr->assets.end(); ++assetCatItr)
      {
         if (performFilter && dStrcmp(assetCatItr->categoryName, filter) != 0)
            continue;

         wxTreeItemId categoryTreeID = AppendItem(currentModuleTreeID, assetCatItr->categoryName, 1, -1, new AssetCategoryTreeItemData(*assetCatItr));
         for (Vector<const AssetDefinition*>::iterator assetItr = assetCatItr->assets.begin(); assetItr != assetCatItr->assets.end(); ++assetItr)
         {
            // Fetch asset definition.
            const AssetDefinition* pAssetDefinition = *assetItr;
            char buf[256];
            dStrcpy(buf, pAssetDefinition->mAssetId);
            const char* moduleName = dStrtok(buf, ":");
            const char* assetName = dStrtok(NULL, ":");
            AppendItem(categoryTreeID, assetName, 2, -1, new AssetTreeItemData(pAssetDefinition));
         }
      }
   }

   // Sort Modules by Name
   SortChildren(mAssetListRoot);
}